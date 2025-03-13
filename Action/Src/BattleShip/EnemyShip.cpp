#include "EnemyShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Common/Assets.h"
#include "UnderBoss/UnderBoss.h"
#include "EnemyAI/TankAI.h"
#include "EnemyAI/HBMAI.h"
#include "Collision/Ray.h"
#include "Player/Player.h"
#include "EnemyAI/EnemyBulletControl.h"

const float EnemyShipRadius_{ 0.8f };
const float EnemyShipHeight_{ 1.f };

const float Hight_{ 1.f };

//各部隊の上限
int Elements_{ 10 };

EnemyShip::EnemyShip(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyShip,Mesh_EnemyShip ,Mesh_EnemyShip ,0 },
	motion_{ 0 },
	motion_Loop_{ true },
	tankais_(10),
	hbmais_(10),
	makeTimer_{ 0.0f } {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "EnemyShip";

	collider_ = BoundingSphere{ EnemyShipRadius_,GSvector3{0,EnemyShipHeight_,0} };

	transform_.position(position);

	transform_.rotate(0, 180, 0);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//敵弾管理クラス
	ebcontrol_ = static_cast<EnemyBulletControl*>(world_->find_actor("EnemyBulletControl"));
}

void EnemyShip::update(float delta_time) {

	//自身の座標を取得
	myPos_ = transform_.position();

	//モーション更新
	mesh_.ChangeMotion(motion_, motion_Loop_);

	//メッシュを更新
	mesh_.Update(delta_time);

	//行列を設定	
	mesh_.Transform(transform_.localToWorldMatrix());

	//AI生成命令
	makeAI(delta_time);

	diecheck();

	//生成フラグが立ったら
	if (world_->gameData()->bossMake() == true) {
		Ray ray = { transform_.position(),-(transform_.up()) };
		spawnPoint_ = myPos_;
		spawnPoint_.y = ray.position.y + Hight_;
		world_->add_actor(new UnderBoss{ world_,spawnPoint_ });
		world_->gameData()->setBossMake(false);
	}
}

void EnemyShip::draw() const {
	mesh_.Draw();

	//collider().draw();
}

void EnemyShip::makeAI(float delta_time) {
	//生成時間更新
	makeTimer_ -= delta_time;

	//生成時間が０になったら
	if (makeTimer_ <= 0) {

		float makedistance = GSvector3::distance(myPos_, player_->transform().position());

		//	優先順位で最低限数生成
		if (nowTank < 3) {
			makeTankAI();
		}
		else if (nowGatling < 1) {
			makeHbmAI(2);
		}
		else if (nowBeamSaber < 1) {
			makeHbmAI(1);
		}
		else if (nowBeamRifle < 3) {
			makeHbmAI(3);
		}
		else if (nowSniper < 1 && makedistance >50) {//戦艦とプレイヤーが離れている
			makeHbmAI(4);
		}
		//	最低限生成し終わったら優先順位はじめから最大数になるまで生成
		else if (nowTank < 5) {
			makeTankAI();
		}
		else if (nowGatling < 2) {
			makeHbmAI(2);
		}
		else if (nowBeamRifle < 5) {
			makeHbmAI(3);
		}
	}
}

void EnemyShip::makeTankAI() {

	//生成座標の設定
	Ray ray = { myPos_,-(transform_.up()) };

	GSvector3 intersect;
	world_->field()->collide(ray, myPos_.y + 30.0f, &intersect);

	spawnPoint_ = myPos_;
	spawnPoint_.y = intersect.y;

	int makenum;

	for (int i = 0; i < Elements_; i++) {

		if (tankais_[i] == NULL) {
			makenum = i;
			break;
		}
	}

	tankais_[makenum] = new TankAI{ world_,spawnPoint_ };
	world_->add_actor(tankais_[makenum]);

	//弾管理クラスに生成したAIを渡す
	ebcontrol_->setTanckAI(tankais_[makenum]);

	//生成時間を入れる
	makeTimer_ = assignmentMakeTimer_;

	nowTank++;
}


void EnemyShip::makeHbmAI(int weapon) {

	//生成座標の設定
	Ray ray = { transform_.position(),-(transform_.up()) };
	spawnPoint_ = myPos_;
	spawnPoint_.y = ray.position.y + Hight_;

	int makenum;

	for (int i = 0; i < Elements_; i++) {

		if (hbmais_[i] == NULL) {
			makenum = i;
			break;
		}
	}

	unsigned int GenwratNum;

	switch (weapon)
	{
	case 1:
		GenwratNum = 3;
		break;
	case 2:
		GenwratNum = 3;
		break;
	case 3:
		GenwratNum = 5;
		break;
	case 4:
		GenwratNum = 3;
		break;
	}

	hbmais_[makenum] = new HBMAI{ world_,spawnPoint_,weapon,GenwratNum };
	world_->add_actor(hbmais_[makenum]);


	//武器に応じて敵弾管理クラスに生成した敵AIを渡す
	if (weapon == 2) {
		ebcontrol_->setGatlingAI(hbmais_[makenum]);
	}
	else if (weapon == 3) {
		ebcontrol_->setBeamLifleAI(hbmais_[makenum]);
	}

	//ランダムな時間を代入
	makeTimer_ = assignmentMakeTimer_;

	switch (weapon)
	{
	case 1:
		nowBeamSaber++;
		break;
	case 2:
		nowGatling++;
		break;
	case 3:
		nowBeamRifle++;
		break;
	case 4:
		nowSniper++;
		break;
	}
}

void EnemyShip::diecheck() {

	for (int i = 0; i < Elements_; i++) {
		if (tankais_[i] == NULL)continue;

		if (tankais_[i]->dieTrigger()) {

			//目的座標による撤退でなければ死亡カウント加算
			if (!tankais_[i]->retreatFrag()) {
				world_->gameData()->setDieEnemyCounter(1);
			}
			tankais_[i]->die();
			tankais_[i] = NULL;
			nowTank--;
		}
	}

	for (int i = 0; i < Elements_; i++) {
		if (hbmais_[i] == NULL)continue;

		if (hbmais_[i]->dieTrigger()) {

			int weapon = hbmais_[i]->myWeapon();
			switch (weapon)
			{
			case 1:
				nowBeamSaber--;
				break;
			case 2:
				nowGatling--;
				break;
			case 3:
				nowBeamRifle--;
				break;
			case 4:
				nowSniper--;
				break;
			}

			hbmais_[i]->die();
			hbmais_[i] = NULL;
			world_->gameData()->setDieEnemyCounter(1);
		}
	}
}