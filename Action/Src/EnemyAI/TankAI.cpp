#include "TankAI.h"
#include "Enemy/Tank.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Player/Player.h"
#include <gslib.h>

//生成数
int MakeNumber = 5;

//戦車管理配列
//std::vector<Actor*> tanks_(MakeNumber);
std::vector<Tank*> tanks_(MakeNumber);

TankAI::TankAI(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "EnemyAI";

	name_ = "TankAI";

	transform_.position(position);

	player = static_cast<Player*>(world_->find_actor("Player"));


	//目標地点のずらしの調整
	distance = 1.0f;

	asignmentdistance = distance;

	//呼び出し回数
	counter = 0;

	//戦車の生成
	MakeTank();

	PTT = 10000;

}

TankAI::~TankAI() {

	//配列内の要素を削除
	//アクターマネージャー側でタンク自体の削除は行われている
	//tanks_.clear();

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

	//0キーが押されたら各タンクを移動状態にする
	//if (gsGetKeyTrigger(GKEY_0)) {

	//	for (int i = 0; i < MakeNumber; i++) {

	//		tanks_[i]->ChangeState(2);
	//	}
	//}


	//時間による制御
	Timer += delta_time;
	MoveTrigger = false;
	//各戦車が移動中かどうか
	for (int i = 0; i < MakeNumber; i++) {

		if (tanks_[i]->StateNow() == 2) {

			//移動中ならフラグをオン
			MoveTrigger = true;
		}
	}

	//一定時間経過かつ移動中フラグがなければ
	if (Timer >= 180 && !MoveTrigger) {

		//プレイヤー座標取得
		Playerpos = player->transform().position();

		for (int i = 0; i < MakeNumber; i++) {

			//タンク座標取得
			TanksPos = tanks_[i]->transform().position();

			//プレイヤーとタンクの距離を取得
			PlayerToTank = sqrt(pow(Playerpos.x - TanksPos.x, 2)
				+ pow(Playerpos.y - TanksPos.y, 2)
				+ pow(Playerpos.z - TanksPos.z, 2));

			//距離採炭が更新されたら
			if (PTT >= PlayerToTank) {
				PTT = PlayerToTank;
			}
		}
		//距離が一定以内なら移動開始
		if (PTT >= 15) {

			for (int j = 0; j < MakeNumber; j++) {
				tanks_[j]->ChangeState(2);
			}
		}
		Timer = 0;
	}


}

void TankAI::draw() const {

}

void TankAI::react(Actor& other) {

}

//合流ポイント
GSvector3 TankAI::MergePoint() const {

	return GSvector3().zero();

}

//プレイヤー付近のポイント
GSvector3 TankAI::NearPlayerPoint() const {

	return GSvector3().zero();

}

//攻撃ポイント
GSvector3 TankAI::AttackPoint()const {

	//再度呼び出されたときの初期化
	if (MakeNumber == counter) {
		counter = 0;
	}

	//旧式
	Playerpos = player->transform().position();

	Playerpos += Playerpos.left() * counter * distance;

	counter++;

	return Playerpos;
}


