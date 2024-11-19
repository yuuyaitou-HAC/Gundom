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

	tag_ = "EnemyAITag";

	name_ = "TankAI";

	transform_.position(position);

	player = static_cast<Player*>(world_->find_actor("Player"));

	enemyship = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));

	//戦車の生成
	MakeTank();

	MinDistance = 10;
	MaxDistance = 20;
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

	//時間による制御
	MoveTimer += delta_time;

	pointtimer -= delta_time;

	Playerpos = player->transform().position();

	//戦車の移動
	MovePoint();

	if (pointtimer <= 0) {
		Updatepoint();
	}

	//戦車の死亡判定
	DieCheack(delta_time);

}

void TankAI::draw() const {

}


bool TankAI::MoveTrigger() {

	//各戦車が移動中かどうか
	for (auto& tank : tanks_) {

		if (tank->StateNow() == 2) {

			return true;
		}
	}
	return false;
}

void TankAI::MovePoint() {

	//一定時間経過かつ移動中フラグがなければ
	if (MoveTimer >= 180 && !MoveTrigger()) {

		for (auto& tank : tanks_) {

			//死亡している個体はスキップ
			if (tank->StateNow() == 6)continue;

			//タンク座標取得
			TanksPos = tank->transform().position();

			//プレイヤーとタンクの距離を取得
			PlayerToTank = GSvector3::distance(Playerpos, TanksPos);


			if (far < PlayerToTank) {
				far = PlayerToTank;
			}
			if (close > PlayerToTank) {
				close = PlayerToTank;
			}

		}
		//距離が一定以内なら移動開始
		if (far > MaxDistance || close < MinDistance) {

			for (auto& tank : tanks_) {

				//死亡している個体はスキップ
				if (tank->StateNow() == 6)continue;

				tank->AttackPoint(AttackPoint());
				tank->ChangeState(2);
				//ここに向かう座標をタンク側に渡す
			}
		}
		MoveTimer = 0;
		far = 0;
		close = 1000;
	}

}

void TankAI::Updatepoint() {

	float distance = GSvector3::distance(Playerpos, TargetPoint);

	if (distance >= MaxDistance) {

		for (auto& tank : tanks_) {

			//死亡している個体や斬撃中の個体は除く
			if (tank->StateNow() == 7)continue;

			tank->AttackPoint(AttackPoint());
		}
	}

	pointtimer = asignmentpointtimer;

}


void TankAI::DieCheack(float timer) {
	for (auto& tank : tanks_) {

		if (tank->StateNow() == 6) {
			DieCounter++;
		}

	}

	if (DieCounter >= 2) {

		for (auto& tank : tanks_) {

			//死んでるやつには命令しない
			if (tank->StateNow() == 6)continue;

			//退却ポイントの設定
			GSvector3 shippos = enemyship->transform().position();
			shippos.y = 1.0f;
			GSvector3 point = shippos;

			tank->AttackPoint(point);
			tank->ChangeState(5);
		}
	}

	if (DieCounter == MakeNumber) {

		for (auto& tank : tanks_) {
			//各タンクの死亡処理
			tank->die();
		}
		Die = true;
	}

	DieCounter = 0;
}

//攻撃ポイント
GSvector3 TankAI::AttackPoint()const {

	//プレイヤー近くにランダムに移動させる
	float max = MaxDistance - 1;

	float min = MaxDistance - 1;

	//ランダムで指定範囲内で座標を出す
	GSvector3 result{ gsRandf(-min,max),0.0f,gsRandf(-min,max) };

	//ランダム座標とプレイヤーの座標を足す
	result += Playerpos;

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
	GSvector3 to_Target = pos - Playerpos;

	//プレイヤーの前ベクトルを求める
	GSvector3 forward = player->transform().forward();

	//各ベクトルのy要素をなくす
	forward.y = to_Target.y = 0.0f;

	//2つのベクトルのなす角度を求める
	float angle = GSvector3::signedAngle(forward, to_Target);

	float distance = GSvector3::distance(pos, Playerpos);

	//指定角度内ならtrueを返し角度外ならfalseを返す
	if (angle <= 45 && angle >= -45 && distance >= MinDistance && MaxDistance >= distance) {
		return true;
	}
	else {
		return false;
	}

}

