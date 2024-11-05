#include "TankAI.h"
#include "Enemy/Tank.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Player/Player.h"
#include <gslib.h>
#include "BattleShip/EnemyShip.h"

//生成数
int MakeNumber = 5;

//ランダム座標の幅
const GSvector2 Range{ 10.0f,10.0f };

TankAI::TankAI(IWorld* world, const GSvector3& position) :
	tanks_(MakeNumber) {

	world_ = world;

	tag_ = "EnemyAI";

	name_ = "TankAI";

	transform_.position(position);

	player = static_cast<Player*>(world_->find_actor("Player"));

	enemyship = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));

	//戦車の生成
	MakeTank();

	PTT = 10000;

}

TankAI::~TankAI() {

	

	//配列内の要素を削除
	//アクターマネージャー側でタンク自体の削除は行われている
	tanks_.clear();

}

void TankAI::MakeTank() {

	//生成座標に自身の座標を代入
	makepos = transform_.position();

	//生成数分戦車を生成
	for (int i = 0; i < MakeNumber; i++) {

		tanks_[i] = new Tank{ world_,makepos };
		world_->add_actor(tanks_[i]);

		makepos.x += 2;

	}

}

void TankAI::update(float delta_time) {

	//1アイドル
	//2移動
	//3攻撃
	//4ダメージ
	//5退避
	//6死

	//時間による制御
	MoveTimer += delta_time;

	//戦車の移動
	MovePoint();

	//戦車の死亡判定
	DieCheack(delta_time);

}

void TankAI::draw() const{

	//gsTextPos(200, 500);
	//gsDrawText("pos = %f,%f,%f", transform_.position().x, transform_.position().y, transform_.position().z);
}


bool TankAI::MoveTrigger() {

	//各戦車が移動中かどうか
	for (int i = 0; i < MakeNumber; i++) {

		if (tanks_[i]->StateNow() == 2) {

			return true;
		}
		else {
			return false;
		}
	}
}

void TankAI::MovePoint() {

	//一定時間経過かつ移動中フラグがなければ
	if (MoveTimer >= 180 && !MoveTrigger()) {

		//プレイヤー座標取得
		Playerpos = player->transform().position();

		for (int i = 0; i < MakeNumber; i++) {

			//タンク座標取得
			TanksPos = tanks_[i]->transform().position();

			//プレイヤーとタンクの距離を取得
			PlayerToTank = GSvector3::distance(Playerpos, TanksPos);

			//距離採炭が更新されたら
			if (PTT >= PlayerToTank) {
				PTT = PlayerToTank;
			}
		}
		//距離が一定以内なら移動開始
		if (PTT >= 10) {

			for (int j = 0; j < MakeNumber; j++) {

				if (tanks_[j]->StateNow() == 6)continue;
				tanks_[j]->AttackPoint(AttackPoint());
				tanks_[j]->ChangeState(2);
				//ここに向かう座標をタンク側に渡す
			}
		}
		MoveTimer = 0;
		PTT = 10000;
	}

}

void TankAI::search() {



}


void TankAI::DieCheack(float timer) {
	for (int i = 0; i < MakeNumber; i++) {

		if (tanks_[i]->StateNow() == 6) {
			DieCounter++;
		}

	}

	if (DieCounter >= 2) {


		for (int i = 0; i < MakeNumber; i++) {

			//死んでるやつには命令しない

			if (tanks_[i]->StateNow() == 6)continue;

			//退却ポイントの設定
			GSvector3 shippos = enemyship->transform().position();
			shippos.y = 1.0f;
			GSvector3 point = shippos;

			tanks_[i]->AttackPoint(point);
			tanks_[i]->ChangeState(5);
		}
		Die = true;
	}
	
	if (DieCounter == MakeNumber) {

		for (int i = 0; i < MakeNumber; i++) {
			//各タンクの死亡処理
			tanks_[i]->die();
		}
		//自身の死亡処理
		die();
	}

	DieCounter = 0;
}

//攻撃ポイント
GSvector3 TankAI::AttackPoint()const {

	//プレイヤー近くにランダムに移動させる
	//ランダムで指定範囲内で座標を出す
	GSvector3 result{ gsRandf(-Range.x,Range.x),0.0f,gsRandf(-Range.y,Range.y) };

	//ランダム座標とプレイヤーの座標を足す
	result += player->transform().position();

	//プレイヤーの視界内なら座標を返し視界外ならこの関数を再度呼ばせる
	if (PTRange(result)) {

		result.y = 1.f;
		return result;
	}
	else {
		return AttackPoint();
	}

}

bool TankAI::dieTrigger()
{
	return Die;
}

//プレイヤー　ランダム　　戦車座標　　プレイヤー
bool TankAI::PTRange(GSvector3 pos) const {

	//ランダム座標とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_Target = pos - player->transform().position();

	//プレイヤーの前ベクトルを求める
	GSvector3 forward = player->transform().forward();

	//各ベクトルのy要素をなくす
	forward.y = to_Target.y = 0.0f;

	//2つのベクトルのなす角度を求める
	float angle = GSvector3::signedAngle(forward, to_Target);

	//指定角度内ならtrueを返し角度外ならfalseを返す
	if (angle <= 45 && angle >= -45) {
		return true;
	}
	else {
		return false;
	}

}

