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

const float EnemyShipRadius_{ 0.8f };
const float EnemyShipHeight_{ 1.f };

const float Hight_{ 1.f };

//各部隊の上限
int Elements_{ 5 };

EnemyShip::EnemyShip(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyShip,Mesh_EnemyShip ,Mesh_EnemyShip ,0 },
	motion_{ 0 },
	Motion_Loop_{ true },
	tankais_(5),
	hbmais_(10) {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "EnemyShip";

	collider_ = BoundingSphere{ EnemyShipRadius_,GSvector3{0,EnemyShipHeight_,0} };

	transform_.position(position);

	transform_.rotate(0, 180, 0);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	MakeTimer_ = 0.0f;


	
	//ゲーム開始時に生成しておく
	//startMake();
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

	//生成時間更新
	MakeTimer_ -= delta_time;

	//生成時間が０になったら
	if (MakeTimer_ <= 0) {

		float makedistance = GSvector3::distance(MyPos_, player_->transform().position());

		//優先順位で最低限数生成
 		if (makeTankCounter < 2) {
			//makeTankAI();
		}
		else if (makeGatlingCounter < 2) {
			//makeHbmAI(2);
		}
		else if (makeBeamRifleCounter < 3) {
			//makeHbmAI(3);
		}
		else if (makeBeamSaberCounter < 1) {
			//makeHbmAI(1);
		}
		else if (makeSniperCounter < 1 && makedistance >50) {//戦艦とプレイヤーが離れている
			//makeHbmAI(4);
		}
		//最低限生成し終わったら優先順位はじめから最大数になるまで生成
		else if (makeTankCounter < 3) {
			//makeTankAI();
		}
		else if (makeGatlingCounter < 3) {
			//makeHbmAI(2);
		}
		else if (makeBeamRifleCounter < 5) {
			//makeHbmAI(3);
		}
	}

	if(gsGetKeyTrigger(GKEY_9))makeHbmAI(1);

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

	//mesh_.Draw();

	collider().draw();
}

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

	//生成時間を入れる
	MakeTimer_ = 360.0f;

	makeTankCounter++;
}


void EnemyShip::makeHbmAI(int weapon) {

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

	hbmais_[makenum] = new HBMAI{ world_,SpawnPoint_,weapon };
	world_->add_actor(hbmais_[makenum]);

	//ランダムな時間を代入
	MakeTimer_ = 360.0f;

	switch (weapon)
	{
	case 1:
		makeBeamSaberCounter++;
		break;
	case 2:
		makeGatlingCounter++;
		break;
	case 3:
		makeBeamRifleCounter++;
		break;
	case 4:
		makeSniperCounter++;
		break;
	}
}

void EnemyShip::diecheck() {

	for (int i = 0; i < Elements_; i++) {
		if (tankais_[i] == NULL)continue;

		if (tankais_[i]->dieTrigger()) {

			tankais_[i]->die();
			tankais_[i] = NULL;
			makeTankCounter--;
			world_->gameData()->setDieEnemyCounter(1);
		}
	}

	for (int i = 0; i < Elements_; i++) {
		if (hbmais_[i] == NULL)continue;

		if (hbmais_[i]->dieTrigger()) {

			int weapon = hbmais_[i]->myWeapon();
			switch (weapon)
			{
			case 1:
				makeBeamSaberCounter--;
				break;
			case 2:
				makeGatlingCounter--;
				break;
			case 3:
				makeBeamRifleCounter--;
				break;
			case 4:
				makeSniperCounter--;
				break;
			}

			hbmais_[i]->die();
			hbmais_[i] = NULL;
			world_->gameData()->setDieEnemyCounter(1);
		}
	}
}
