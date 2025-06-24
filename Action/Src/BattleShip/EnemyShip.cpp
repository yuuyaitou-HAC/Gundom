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
#include "GSmath.h"

//各部隊の上限
int TankElements_{ 10 };
int HBMElements_{ 15 };

EnemyShip::EnemyShip(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyShip,Mesh_EnemyShip ,Mesh_EnemyShip ,0 },
	tankais_(TankElements_),
	hbmais_(HBMElements_),
	makeTimer_{ 0.0f },
	beamSaber_(10),
	gatling_(10),
	beamRifle_(10) {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "EnemyShip";

	collider_ = BoundingSphere{ enemyShipRadius_,GSvector3{0,enemyShipHeight_,0} };

	transform_.position(position);

	//正面(プレイヤー)を向かせる
	transform_.rotate(0, 180, 0);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//敵弾管理クラス
	ebcontrol_ = static_cast<EnemyAttackControl*>(world_->find_actor("EnemyBulletControl"));

	isDrawEffect_ = true;

	//バーニア
	vernierEffect1_ = gsPlayEffect(Effect_VernierBL, &position);
	vernierEffect2_ = gsPlayEffect(Effect_VernierBL, &position);
	vernierEffect3_ = gsPlayEffect(Effect_VernierBL, &position);
}

//更新
void EnemyShip::update(float delta_time) {

	//自身の座標を取得
	myPos_ = transform_.position();

	//メッシュを更新
	mesh_.Update(delta_time);

	//行列を設定	
	mesh_.Transform(transform_.localToWorldMatrix());

	//AI生成命令
	make_AI(delta_time);

	//死亡チェック
	die_check();

	//生成フラグが立ったら
	if (world_->gameData()->underBossMake() == true) {
		Ray ray = { transform_.position(),-(transform_.up()) };
		spawnPoint_ = myPos_;
		spawnPoint_.y = ray.position.y + makeHeight_;
		world_->add_actor(new UnderBoss{ world_,spawnPoint_ });
		world_->gameData()->setUnderBossMake(false);
	}

	if (world_->gameData()->bossMake() == true) {
		//ボス
		world_->add_actor(new Boss{ world_,bossMakePos_ });
		world_->gameData()->setBossMake(false);
	}

	//移動
	move(delta_time);

	//エフェクトの更新
	effect_update();
}

//描画
void EnemyShip::draw() const {
	//メッシュ描画
	mesh_.Draw();
}

//エフェクトの更新
void EnemyShip::effect_update() {
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

	playerDistance_ = GSvector3::distance(effectDrawPos_, playerPos_);

	//プレイヤーの距離に応じて描画する
	if (playerDistance_ >= effectDrawDistance_.x || playerDistance_ <= effectDrawDistance_.y) {
		if (isDrawEffect_) {
			//地面の砂埃
			dustEffect_ = gsPlayEffect(Effect_dust, &myPos_);
			isDrawEffect_ = false;
		}
	}
	else {
		//エフェクト停止
		gsStopEffect(dustEffect_);
		isDrawEffect_ = true;
	}
	dustEffectPos_ = transform_.position();
	dustEffectPos_.y = -8;
	localMatrix_ = GSmatrix4::TRS(dustEffectPos_, GSquaternion::euler(dustEffetEuler_), dustEffectScale_);
	gsSetEffectMatrix(dustEffect_, &localMatrix_);

	//色の変更
	gsSetEffectColor(dustEffect_, &dustColor_);
}

//移動
void EnemyShip::move(float delta_time) {
	timeElapsed_ += delta_time;

	// y軸方向にsinカーブで上下する値を生成
	float offsetY = std::sin(timeElapsed_ * frequency_ * GS_PI) * amplitude_;

	// 現在の高さに加算して位置を更新
	GSvector3 moveposition = basePosition_;  // 移動の基準位置
	moveposition.y += offsetY;

	transform_.translate(moveposition * delta_time);
}

//AI生成
void EnemyShip::make_AI(float delta_time) {
	//生成時間更新
	makeTimer_ -= delta_time;

	//生成時間が０になったら
	if (makeTimer_ <= 0) {
		float playerDistance = GSvector3::distance(myPos_, player_->transform().position());

		//	優先順位で最低限数生成
		if (nowTank_ < minMakeTank_) {
			make_tankAI();
		}
		else if (nowGatling_ < minMakeGatling_) {
			make_hbmAI(EnemyShip::MakeHBMWeapon::Gatling);
		}
		else if (nowBeamSaber_ < minMakeBeamSaber_) {
			make_hbmAI(EnemyShip::MakeHBMWeapon::BeamSaber);
		}
		else if (nowBeamRifle_ < minMakeBeamRifle_) {
			make_hbmAI(EnemyShip::MakeHBMWeapon::BeamRifle);
		}
		else if (nowSniper_ < minMakeSniper_ && playerDistance >sniperMakeDistnace_) {//戦艦とプレイヤーが離れている
			make_hbmAI(EnemyShip::MakeHBMWeapon::Sniper);
		}
		//	最低限生成し終わったら優先順位はじめから最大数になるまで生成
		else if (nowTank_ < maxMakeTank_) {
			make_tankAI();
		}
		else if (nowGatling_ < maxMakeGatling_) {
			make_hbmAI(EnemyShip::MakeHBMWeapon::Gatling);
		}
		else if (nowBeamRifle_ < maxMakeBeamRifle_) {
			make_hbmAI(EnemyShip::MakeHBMWeapon::BeamRifle);
		}
	}
}

//戦車AI生成
void EnemyShip::make_tankAI() {

	//生成座標の設定
	Ray ray = { myPos_,-(transform_.up()) };

	GSvector3 intersect;
	world_->field()->collide(ray, myPos_.y + 30.0f, &intersect);
	spawnPoint_ = myPos_;
	spawnPoint_.y = intersect.y;

	int makenum = -1;

	for (int i = 0; i < TankElements_; i++) {

		if (tankais_[i] == NULL) {
			makenum = i;
			break;
		}
	}
	if (makenum == -1)return;

	tankais_[makenum] = new TankAI{ world_,spawnPoint_ };
	world_->add_actor(tankais_[makenum]);

	//弾管理クラスに生成したAIを渡す
	ebcontrol_->set_tankAI(tankais_[makenum]);

	//生成時間を入れる
	makeTimer_ = assignmentMakeTimer_;

	nowTank_++;
}

//HBMAI生成
void EnemyShip::make_hbmAI(EnemyShip::MakeHBMWeapon makehbm) {

	//生成座標の設定
	Ray ray = { myPos_,-(transform_.up()) };

	GSvector3 intersect;
	world_->field()->collide(ray, myPos_.y + 30.0f, &intersect);

	spawnPoint_ = myPos_;
	spawnPoint_.y = intersect.y;

	int makenum = -1;

	for (int i = 0; i < HBMElements_; i++) {

		if (hbmais_[i] == NULL) {
			makenum = i;
			break;
		}
	}
	if (makenum == -1)return;

	//武器ごとで生成する種類を変える
	switch (makehbm)
	{
	case EnemyShip::MakeHBMWeapon::BeamSaber:

		hbmais_[makenum] = new HBMAI{ world_,spawnPoint_,HBMAI::Weapon::BeamSaber,beamSaberUnitNum_ };
		break;
	case EnemyShip::MakeHBMWeapon::Gatling:

		hbmais_[makenum] = new HBMAI{ world_,spawnPoint_,HBMAI::Weapon::Gatling,gatlingUnitNum_ };
		break;
	case EnemyShip::MakeHBMWeapon::BeamRifle:

		hbmais_[makenum] = new HBMAI{ world_,spawnPoint_,HBMAI::Weapon::BeamRifle,BeamRifleUnitNum_ };
		break;
	case EnemyShip::MakeHBMWeapon::Sniper:

		hbmais_[makenum] = new HBMAI{ world_,spawnPoint_,HBMAI::Weapon::Sniper,SniperUnitNum_ };
		break;
	}
	world_->add_actor(hbmais_[makenum]);

	//武器に応じて敵弾管理クラスに生成した敵AIを渡す
	if (makehbm == EnemyShip::MakeHBMWeapon::Gatling) {
		ebcontrol_->set_gatlingAI(hbmais_[makenum]);
	}
	else if (makehbm == EnemyShip::MakeHBMWeapon::BeamRifle) {
		ebcontrol_->set_beamRifleAI(hbmais_[makenum]);
	}

	//ランダムな時間を代入
	makeTimer_ = assignmentMakeTimer_;

	//生成した個体を合計数に加える
	switch (makehbm)
	{
	case EnemyShip::MakeHBMWeapon::BeamSaber:
		nowBeamSaber_++;
		break;
	case EnemyShip::MakeHBMWeapon::Gatling:
		nowGatling_++;
		break;
	case EnemyShip::MakeHBMWeapon::BeamRifle:
		nowBeamRifle_++;
		break;
	case EnemyShip::MakeHBMWeapon::Sniper:
		nowSniper_++;
		break;
	}
}

//死亡確認
void EnemyShip::die_check() {

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

			switch (hbmais_[i]->myWeapon())
			{
			case HBMAI::Weapon::BeamSaber:
				nowBeamSaber_--;
				break;
			case HBMAI::Weapon::Gatling:
				nowGatling_--;
				break;
			case HBMAI::Weapon::BeamRifle:
				nowBeamRifle_--;
				break;
			case HBMAI::Weapon::Sniper:
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