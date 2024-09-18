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
	MakeNumber = 1;

	//戦車の生成
	MakeTank();

	//目標地点のずらしの調整
	Adjustment = 4.0f;

	counter = 0;

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

	//旧

	////呼び出された回数が生成されている戦車と同じだったら初期化
	//if (MakeNumber == counter) {
	//	counter = 0;
	//}

	////目標地点入力
	//Playerpos = player->transform().position();

	////二回目以降なら左にずらす
	//Playerpos += Playerpos.left() * counter * Adjustment;

	////呼び出し回数を更新
	//counter++;


	//新


	int jud = 1;

	if (MakeNumber == counter) {
		counter = 0;
	}


	//生成数が奇数の時の場合
	if (MakeNumber % 2 == 1) {

		//左右に配置していく
		if (counter >= 1) {
			if (counter / 2 % 1) {
				jud = 1;
			}
			else {
				jud = -1;
			}
		}

		//2番目以降の場合
		if (counter >= 1) {

		}
		//最初の移動の場合
		else if(counter == 0){
			Playerpos = player->transform().position();
		}

		//												カウント　距離
		//Playerpos += (player->transform().position() * counter * Adjustment) * jud;

	}

	counter++;

	return Playerpos;
}


