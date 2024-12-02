#include "EnemyShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "BOSS/Boss.h"
#include "EnemyAI/TankAI.h"
#include "EnemyAI/HBMAI.h"
#include "Collision/Ray.h"
#include "Player/Player.h"

#include <random>

const float EnemyShipRadius_{ 0.8f };
const float EnemyShipHeight_{ 1.f };

const float Hight_{ 1.f };

//一部隊の個数
int Elements_{ 10 };

EnemyShip::EnemyShip(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyShip,Mesh_EnemyShip ,Mesh_EnemyShip ,0 },
	motion_{ 0 },
	Motion_Loop_{ true },
	MaximumNumberGenerated_{ 10 },
	tankais_(Elements_),
	hbmais_(Elements_) {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "EnemyShip";

	collider_ = BoundingSphere{ EnemyShipRadius_,GSvector3{0,EnemyShipHeight_,0} };

	transform_.position(position);

	transform_.rotate(0, 180, 0);

	//ゲーム開始時に生成しておく
	startMake();
}

void EnemyShip::update(float delta_time) {

	//自身の座標を取得
	MyPos_ = transform_.position();

	//モーション更新
	mesh_.ChangeMotion(motion_, Motion_Loop_);

	//メッシュを更新
	mesh_.Update(delta_time);

	//行列を設定	
	mesh_.Transform(transform_.localToWorldMatrix());

	MakeTimer_ -= delta_time;

	if (MakeTimer_ <= 0 && MakeCounter_ < MaximumNumberGenerated_ && world_->gameData()->bossMake() == false) {
		//生成するものをランダムで決める
		int randomWeapon = gsRand(1, 2);

		switch (randomWeapon)
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

	//生成フラグが立ったら
	if (world_->gameData()->bossMake() == true) {
		Ray ray = { transform_.position(),-(transform_.up()) };
		SpawnPoint_ = MyPos_;
		SpawnPoint_.y = ray.position.y + Hight_;
		world_->add_actor(new Boss{ world_,SpawnPoint_ });
		world_->gameData()->setBossMake(false);
	}
}

void EnemyShip::draw() const {

	mesh_.Draw();

	collider().draw();
}

void EnemyShip::react(Actor& other) {}

void EnemyShip::startMake() {


	int makenum;

	for (int i = 0; i < 2; i++) {

		//一体目の生成
		for (int i = 0; i < Elements_; i++) {

			if (tankais_[i] == NULL) {
				makenum = i;
				break;
			}
		}
		tankais_[makenum] = new TankAI{ world_,pos[i] };
		world_->add_actor(tankais_[makenum]);
		MakeCounter_++;
		makenum = 0;

	}

	for (int i = 0; i < 2; i++) {
		for (int i = 0; i < Elements_; i++) {

			if (hbmais_[i] == NULL) {
				makenum = i;
				break;
			}
		}

		hbmais_[makenum] = new HBMAI{ world_,pos[i + 2],gsRand(1,3) };
		world_->add_actor(hbmais_[makenum]);
		MakeCounter_++;
		makenum = 0;
	}

}

void EnemyShip::makeTankAI() {

	//生成座標の設定
	Ray ray = { transform_.position(),-(transform_.up()) };
	SpawnPoint_ = MyPos_;
	SpawnPoint_.y = ray.position.y + Hight_;

	int makenum;

	for (int i = 0; i < Elements_; i++) {

		if (tankais_[i] == NULL) {
			makenum = i;
			break;
		}
	}

	tankais_[makenum] = new TankAI{ world_,SpawnPoint_ };
	world_->add_actor(tankais_[makenum]);

	//ランダムな時間を代入
	MakeTimer_ = gsRand(MakeTimerRand_.x, MakeTimerRand_.y);

	MakeCounter_++;


}

//武器生成の確率
int EnemyShip::randWeapon() {

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	float distance = GSvector3::distance(transform_.position(), player_->transform().position());

	int random = gsRand(1, 100);

	//プレイヤーと戦艦の距離が一定数あれば
	if (distance > 50) {

		if (random <= 30) {
			return 1;
		}
		else if (random <= 60) {
			return 2;
		}
		else if (random <= 85) {
			return 3;
		}
		else {
			return 4;
		}

	}
	//プレイヤーと戦艦が近かったら
	else {
		if (random <= 40) {
			return 1;
		}
		else if (random <= 80) {
			return 2;
		}
		else {
			return 3;
		}
	}
}

void EnemyShip::makeHbmAi() {

	//生成座標の設定
	Ray ray = { transform_.position(),-(transform_.up()) };
	SpawnPoint_ = MyPos_;
	SpawnPoint_.y = ray.position.y + Hight_;

	int makenum;

	for (int i = 0; i < Elements_; i++) {

		if (hbmais_[i] == NULL) {
			makenum = i;
			break;
		}
	}

	hbmais_[makenum] = new HBMAI{ world_,SpawnPoint_,randWeapon() };
	world_->add_actor(hbmais_[makenum]);


	//ランダムな時間を代入
	MakeTimer_ = gsRand(MakeTimerRand_.x, MakeTimerRand_.y);

	MakeCounter_++;

}

void EnemyShip::diecheck() {

	for (int i = 0; i < Elements_; i++) {
		if (tankais_[i] == NULL)continue;

		if (tankais_[i]->dieTrigger()) {

			tankais_[i]->die();
			tankais_[i] = NULL;
			MakeCounter_--;
			world_->gameData()->setDieEnemyCounter(1);
		}
	}

	for (int i = 0; i < Elements_; i++) {
		if (hbmais_[i] == NULL)continue;

		if (hbmais_[i]->dieTrigger()) {

			hbmais_[i]->die();
			hbmais_[i] = NULL;
			MakeCounter_--;
			world_->gameData()->setDieEnemyCounter(1);
		}
	}

}
