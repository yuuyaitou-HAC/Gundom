#include "EnemyShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Common/Assets.h"
#include "UnderBoss/UnderBoss.h"
#include "Boss/Boss.h"
#include "EnemyAI/TankAI.h"
#include "EnemyAI/HBMAI.h"
#include "Collision/Ray.h"
#include "Player/Player.h"
#include "EnemyAI/EnemyAttackControl.h"
#include "GSeffect.h"
#include "GSeffect.h"

//各部隊の上限
int TankElements_{ 10 };
int HBMElements_{ 15 };

EnemyShip::EnemyShip(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyShip,Mesh_EnemyShip ,Mesh_EnemyShip ,0 },
	motion_{ 0 },
	motion_Loop_{ true },
	tankais_(TankElements_),
	hbmais_(HBMElements_),
	makeTimer_{ 0.0f },
	beamSaber_(10),
	gatring_(10),
	beamRifle_(10) {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "EnemyShip";

	collider_ = BoundingSphere{ enemyShipRadius_,GSvector3{0,enemyShipHeight_,0} };

	transform_.position(position);

	transform_.rotate(0, 180, 0);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//敵弾管理クラス
	ebcontrol_ = static_cast<EnemyAttackControl*>(world_->find_actor("EnemyBulletControl"));

	effectTrigger_ = true;

	//バーニア
	vernierEffect1_ = gsPlayEffect(Effect_VernierBL, &position);
	vernierEffect2_ = gsPlayEffect(Effect_VernierBL, &position);
	vernierEffect3_ = gsPlayEffect(Effect_VernierBL, &position);
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
	if (world_->gameData()->underBossMake() == true) {
		Ray ray = { transform_.position(),-(transform_.up()) };
		spawnPoint_ = myPos_;
		spawnPoint_.y = ray.position.y + makeHight_;
		world_->add_actor(new UnderBoss{ world_,spawnPoint_ });
		world_->gameData()->setUnderBossMake(false);
	}

	if (world_->gameData()->bossMake() == true) {
		//ボス
		world_->add_actor(new Boss{ world_,GSvector3{-200,10,1.5} });
		world_->gameData()->setBossMake(false);
	}

	move(delta_time);

	//エフェクトの更新
	localMatrix_ = GSmatrix4::TRS(vernierEffectPos1_, GSquaternion::euler(vernierEffectEuler_), vernierEffectScale_);
	effectWorld_ = localMatrix_ * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernierEffect1_, &effectWorld_);

	localMatrix_ = GSmatrix4::TRS(vernierEffectPos2_, GSquaternion::euler(vernierEffectEuler_), vernierEffectScale_);
	effectWorld_ = localMatrix_ * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernierEffect2_, &effectWorld_);

	localMatrix_ = GSmatrix4::TRS(vernierEffectPos3_, GSquaternion::euler(vernierEffectEuler_), vernierEffectScale_);
	effectWorld_ = localMatrix_ * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernierEffect3_, &effectWorld_);

	//それぞれの座標取得
	playerPos_ = player_->transform().position();
	effectDrawPos_ = transform_.position();
	playerPos_.y = effectDrawPos_.y = 0.0f;

	//プレイヤーの距離に応じて描画する
	if (GSvector3::distance(effectDrawPos_, playerPos_) <= 100 && effectTrigger_) {
		//地面の砂埃
		dustEffect_ = gsPlayEffect(Effect_dust, &myPos_);
		effectTrigger_ = false;
	}
	if (GSvector3::distance(effectDrawPos_, playerPos_) > 100) {
		gsStopEffect(dustEffect_);

		effectTrigger_ = true;
	}
	dustEffectPos_ = transform_.position();
	dustEffectPos_.y = -8;
	localMatrix_ = GSmatrix4::TRS(dustEffectPos_, GSquaternion::euler(dustEffetEuler_), dustEffectScale_);
	gsSetEffectMatrix(dustEffect_, &localMatrix_);

	//色の変更
	gsSetEffectColor(dustEffect_, &dustColor_);
}

void EnemyShip::draw() const {
	mesh_.Draw();
}

//移動
void EnemyShip::move(float delta_time) {
	timeElapsed_ += delta_time;

	// y軸方向にsinカーブで上下する値を生成
	float offsetY = std::sin(timeElapsed_ * frequency_ * 3.14f ) * amplitude_;

	// 現在の高さに加算して位置を更新
	GSvector3 moveposition = basePosition_;  // 移動の基準位置
	moveposition.y += offsetY;

	transform_.translate(moveposition * delta_time);
}

void EnemyShip::makeAI(float delta_time) {
	//生成時間更新
	makeTimer_ -= delta_time;

	//生成時間が０になったら
	if (makeTimer_ <= 0)mission1MakeAi();
}

//ミッション1での生成
void EnemyShip::mission1MakeAi() {

	float makedistance = GSvector3::distance(myPos_, player_->transform().position());

	//	優先順位で最低限数生成
	if (nowTank_ < 3) {
		makeTankAI();
	}
	else if (nowGatling_ < 1) {
		makeHbmAI(2);
	}
	else if (nowBeamSaber_ < 1) {
		makeHbmAI(1);
	}
	else if (nowBeamRifle_ < 3) {
		makeHbmAI(3);
	}
	else if (nowSniper_ < 1 && makedistance >50) {//戦艦とプレイヤーが離れている
		makeHbmAI(4);
	}
	//	最低限生成し終わったら優先順位はじめから最大数になるまで生成
	else if (nowTank_ < 5) {
		makeTankAI();
	}
	else if (nowGatling_ < 2) {
		makeHbmAI(2);
	}
	else if (nowBeamRifle_ < 5) {
		makeHbmAI(3);
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

	for (int i = 0; i < TankElements_; i++) {

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

	nowTank_++;
}


void EnemyShip::makeHbmAI(int weapon) {

	//生成座標の設定
	Ray ray = { myPos_,-(transform_.up()) };

	GSvector3 intersect;
	world_->field()->collide(ray, myPos_.y + 30.0f, &intersect);

	spawnPoint_ = myPos_;
	spawnPoint_.y = intersect.y;

	int makenum;

	for (int i = 0; i < HBMElements_; i++) {

		if (hbmais_[i] == NULL) {
			makenum = i;
			break;
		}
	}

	//武器ごとで部隊構成人数を決める
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
		nowBeamSaber_++;
		break;
	case 2:
		nowGatling_++;
		break;
	case 3:
		nowBeamRifle_++;
		break;
	case 4:
		nowSniper_++;
		break;
	}
}

void EnemyShip::diecheck() {

	for (int i = 0; i < TankElements_; i++) {
		if (tankais_[i] == NULL)continue;

		if (tankais_[i]->dieTrigger()) {

			//目的座標による撤退でなければ死亡カウント加算
			if (!tankais_[i]->retreatFrag()) {
				world_->gameData()->setDieEnemyCounter(1);
			}
			tankais_[i]->die();
			tankais_[i] = NULL;
			nowTank_--;
		}
	}

	for (int i = 0; i < HBMElements_; i++) {
		if (hbmais_[i] == NULL)continue;

		if (hbmais_[i]->dieTrigger()) {

			int weapon = hbmais_[i]->myWeapon();
			switch (weapon)
			{
			case 1:
				nowBeamSaber_--;
				break;
			case 2:
				nowGatling_--;
				break;
			case 3:
				nowBeamRifle_--;
				break;
			case 4:
				nowSniper_--;
				break;
			}
			if (!hbmais_[i]->retreatFrag()) {
				world_->gameData()->setDieEnemyCounter(1);
			}
			hbmais_[i]->die();
			hbmais_[i] = NULL;
		}
	}
}