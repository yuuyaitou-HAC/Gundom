#include "TankAI.h"
#include "Enemy/Tank.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Player/Player.h"

TankAI::TankAI(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "EnemyAI";

	name_ = "TankAI";

	transform_.position(position);

	player = static_cast<Player*>(world_->find_actor("Player"));

	//生成数
	MakeNumber = 3;

	//戦車の生成
	MakeTank();

	//目標地点のずらしの調整
	Adjustment = 1.5f;

}

void TankAI::MakeTank() {

	//生成座標に自身の座標を代入
	makepos = transform_.position();

	//生成数分戦車を生成
	for (int i = 0; i < MakeNumber; i++) {

		world_->add_actor(new Tank{ world_,makepos });

		makepos.x += 2;

	}

}

void TankAI::update(float delta_time) {

}

void TankAI::draw() const {

}

void TankAI::react(Actor& other) {

}

//目標地点を返す
GSvector3 TankAI::point()const {

	//呼び出された回数が生成されている戦車と同じだったら初期化
	if (MakeNumber == counter) {
		counter = 0;
	}

	//目標地点入力
	pos = player->transform().position();

	//二回目以降なら左にずらす
	pos += pos.left() * counter * Adjustment;

	//呼び出し回数を更新
	counter++;

	return pos;
}


