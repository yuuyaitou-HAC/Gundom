#include "EnemyShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "BOSS/Boss.h"
#include "EnemyAI/TankAI.h"
#include "EnemyAI/HBMAI.h"
#include "Collision/Ray.h"


const float EnemyShipRadius_{ 0.8f };
const float EnemyShipHeight_{ 1.f };

const float Hight_{ 1.f };

//一部隊の個数
int elements{ 5 };

//ボス生成に必要なKILL数
int MakeBossCounter{ 3 };

EnemyShip::EnemyShip(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyShip,Mesh_EnemyShip ,Mesh_EnemyShip ,0 },
	motion_{ 0 },
	motion_loop_{ true },
	MaximumNumberGenerated{ 10 },
	tankais_(elements),
	hbmais_(elements) {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "EnemyShip";

	collider_ = BoundingSphere{ EnemyShipRadius_,GSvector3{0,EnemyShipHeight_,0} };

	transform_.position(position);

	boss_ = static_cast<Boss*>(world_->find_actor("Boss"));

}

void EnemyShip::update(float delta_time) {

	//自身の座標を取得
	pos = transform_.position();

	//モーション更新
	mesh_.ChangeMotion(motion_, motion_loop_);

	//メッシュを更新
	mesh_.Update(delta_time);

	//行列を設定	
	mesh_.Transform(transform_.localToWorldMatrix());


	MakeTimer -= delta_time;

	if (MakeTimer <= 0 && makeCounter < MaximumNumberGenerated) {
		//生成するものをランダムで決める
		int a = gsRand(makerand.x, makerand.y);

		switch (a)
		{
		case 1:
			makeTankAI();
			break;
		case 2:
			makeHbmAi();
			break;
		}
	}

	diecheck();

	//一定数殺したらボス生成
	if (!bossmake && diecounter >= MakeBossCounter) {
		Ray ray = { transform_.position(),-(transform_.up()) };
		Spawnpoint = pos;
		Spawnpoint.y = ray.position.y + Hight_;
		world_->add_actor(new Boss{ world_,Spawnpoint });

		bossmake = true;
	}

}

void EnemyShip::draw() const {

	mesh_.Draw();

	gsTextPos(100, 500);
	gsDrawText("makecounter = %d", makeCounter);

}

void EnemyShip::react(Actor& other) {

}


void EnemyShip::makeTankAI() {

	//生成座標の設定
	Ray ray = { transform_.position(),-(transform_.up()) };
	Spawnpoint = pos;
	Spawnpoint.y = ray.position.y + Hight_;

	int makenum;

	for (int i = 0; i < elements; i++) {

		if (tankais_[i] == NULL) {
			makenum = i;
			break;
		}

	}

	tankais_[makenum] = new TankAI{ world_,Spawnpoint };
	world_->add_actor(tankais_[makenum]);


	//ランダムな時間を代入
	MakeTimer = gsRand(MakeTimerRand.x, MakeTimerRand.y);

	makeCounter++;


}

void EnemyShip::makeHbmAi() {


	//生成座標の設定
	Ray ray = { transform_.position(),-(transform_.up()) };
	Spawnpoint = pos;
	Spawnpoint.y = ray.position.y + Hight_;

	int makenum;

	for (int i = 0; i < elements; i++) {

		if (hbmais_[i] == NULL) {
			makenum = i;
			break;
		}

	}

	hbmais_[makenum] = new HBMAI{ world_,Spawnpoint };
	world_->add_actor(hbmais_[makenum]);


	//ランダムな時間を代入
	MakeTimer = gsRand(MakeTimerRand.x, MakeTimerRand.y);

	makeCounter++;

}

void EnemyShip::diecheck() {

	for (int i = 0; i < elements; i++) {
		if (tankais_[i] == NULL)continue;

		if (tankais_[i]->dieTrigger()) {

			tankais_[i]->die();
			tankais_[i] = NULL;
			makeCounter--;
			diecounter++;
		}
	}

	for (int i = 0; i < elements; i++) {
		if (hbmais_[i] == NULL)continue;

		if (hbmais_[i]->dieTrigger()) {

			hbmais_[i]->die();
			hbmais_[i] = NULL;
			makeCounter--;
			diecounter++;
		}
	}

}
