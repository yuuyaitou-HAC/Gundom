#include "TankAI.h"
#include "Enemy/Tank.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Player/Player.h"


TankAi::TankAi(IWorld* world, const GSvector3& position) {

	makeNumber = 1;

	world_ = world;

	tag_ = "EnemyAITag";

	name_ = "TankAI";

	//プレイヤー取得
	player = static_cast<Player*>(world_->find_actor("Player"));

	pos = transform_.position();

	//Playerpos = player->transform().position();

	//戦車の生成
	MakeTank();
}

//戦車の生成
void TankAi::MakeTank() {

	//生成数分戦車を生成する
	for (int i = 0; i < makeNumber; i++) {

		//タンクを生成
		world_->add_actor(new Tank{ world_,pos });



		//横一列に生成
		pos += pos.right() * 2;

	}
}


void TankAi::update(float delta_time) {




}

void TankAi::draw() const {

}

void TankAi::react(Actor& other) {


}

float TankAi::Position()
{
	player = static_cast<Player*>(world_->find_actor("Player"));

	//プレイヤーの座標取得
	Destination = player->transform().position();

	//自身とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_target = Destination - transform_.position();
	//自身の前ベクトルを求める
	GSvector3 forward = transform_.forward();

	//ベクトルのy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;

	return GSvector3::signedAngle(forward, to_target);
}

