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
std::vector<Actor*> tanks_(MakeNumber);

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

	//呼び出し回数(奇数番目)
	a = 1;

	//戦車の生成
	MakeTank();
}

TankAI::~TankAI() {

	//for (int i = 0; i < MakeNumber; i++) {

	//配列内の要素を削除
	tanks_.clear();
	//}

}

void TankAI::MakeTank() {

	//生成座標に自身の座標を代入
	makepos = transform_.position();

	//生成数分戦車を生成
	for (int i = 0; i < MakeNumber; i++) {

		//out of range発生
		tanks_[i] = new Tank{ world_,makepos };
		world_->add_actor(tanks_[i]);

		makepos.x += 2;

	}

}

void TankAI::update(float delta_time) {


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


