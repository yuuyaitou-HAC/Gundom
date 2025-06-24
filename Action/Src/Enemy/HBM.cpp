#include "HBM.h"
#include "Collision/AttackCollider.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "Collision/BasicAttackCollider.h"
#include "UnderBossBullet/UnderBossBeamRifleBullet.h"
#include "EnemyBullet/GatlingBullet.h"
#include "EnemyBullet/EnemyAttackRange.h"
#include "EnemyBullet/SniperBullet.h"
#include "GSeffect.h"

enum {

	//アイドルモーション
	Motion_Idle_G = 0,
	Motion_Idle_A = 1,
	Motion_Idle_SaberEarth = 2,

	//銃装備時の移動
	Motion_WarkF_G = 3,
	Motion_WarkB_G = 4,
	Motion_WarkL_G = 5,
	Motion_WarkR_G = 6,

	//銃装備時の空中移動
	Motion_WarkF_A = 7,
	Motion_WarkB_A = 8,
	Motion_WarkL_A = 9,
	Motion_WarkR_A = 10,

	//剣装備時の移動
	Motion_WarkF_SaberEarth = 11,
	Motion_WarkB_SaberEarth = 12,
	Motion_WarkL_SaberEarth = 13,
	Motion_WarkR_SaberEarth = 14,

	//銃装備時のその場での攻撃
	Motion_Attack_GunEarth = 15,

	//銃装備時のその場での攻撃(空中)
	Motion_Attack1_GunAir = 16,

	//剣装備時の攻撃(コンボ含む)
	Motion_Attack1_SubarEath = 17,

	//銃装備時のジャンプ
	Motion_JumpStart_GunEarth = 18,
	Motion_Jump_GunEarth = 19,
	Motion_JumpEnd_GunEarth = 20,

	//銃装備時の着地
	Motion_Landing_GunEarth = 21,

	//銃装備時の地上でダメージを受けたとき
	Motion_Damage_GunEarth = 22,

	//銃装備時の空中でダメージを受けたとき
	Motion_Damage_GunAir = 23,

	//剣装備時の地上でダメージを受けたとき
	Motion_Damage1_SaberEarth = 24,

	//銃装備時に死んだ
	Motion_Die_GunEarth = 25,

	//銃装備時に空中で死んだ
	Motion_Die_GunAir = 26,

	//剣装備時に死んだ
	Motion_Die_SaberEarth = 27,
};

//コンストラクタ
HBM::HBM(IWorld* world, const GSvector3& position, HBM::Weapon weapon) :
	mesh_{ Mesh_HBM,Mesh_HBM,Mesh_HBM,Motion_Idle_G,true },
	motion_{ Motion_Idle_G },
	motionLoop_{ true },
	state_{ State::Idle },
	player_{ nullptr },
	drawMeshFrag_{ true } {

	world_ = world;

	tag_ = "EnemyTag";
	name_ = "HBM";

	collider_ = BoundingSphere{ radius_,GSvector3{0.f,height_,0.f} };

	transform_.position(position);

	mesh_.Transform(transform_.localToWorldMatrix());

	damage_ = 0;

	//プレイヤーを取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//攻撃の間隔を代入
	attackTimer_ = gsRand(randSlashTime_.x, randSlashTime_.y);

	attackMoveTimer_ = 0.0f;

	fnishSlashTimer_ = fnishSlashTimeAssignment_;

	Weapon_ = weapon;

	//武器と同じデータを見つける
	auto it = enemyState_.find(Weapon_);

	//各ステータスにデータを入れる
	if (it != enemyState_.end()) {
		health_ = it->second.x;
		attackValue_ = it->second.y;
		defensive_ = it->second.z;
	}
}

//更新
void HBM::update(float delta_time) {

	//距離に応じてエフェクト再生するかどうかのフラグを変える
	if (player_distance() >= 30)playEffectDistance_ = false;
	else playEffectDistance_ = true;

	update_state(delta_time);

	if (!fryTrigger_) {
		//重力
		velocity_.y += gravity_ * delta_time;
		transform_.translate(0.f, velocity_.y, 0.f);
	}

	collide_field();

	mesh_.ChangeMotion(motion_, motionLoop_);

	mesh_.Update(delta_time);

	mesh_.Transform(transform_.localToWorldMatrix());

	//自身の座標
	myPos_ = transform_.position();

	playerPos_ = player_->transform().position();
	playerPos_.y += playerOffsetY_;

	//エフェクトの更新
	effectUpdate(delta_time);

	//バーニアエフェクトのコントロール
	vernierstop();
}

//描画
void HBM::draw() const {
	if (drawMeshFrag_)mesh_.Draw();
}

//エフェクトの更新
void HBM::effectUpdate(float delta_time) {

	GSmatrix4 world;
	GSmatrix4 local_matrix;

	//バーニアエフェクト
	local_matrix = GSmatrix4::TRS(VernierEffectPos_, GSquaternion::euler(VernierEffectEuler_), VernierEffectScale_);
	world = local_matrix * mesh_.BoneMatrices(4);
	gsSetEffectMatrix(vernierEffect_, &world);
}

//当たり判定
void HBM::react(Actor& other) {
	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Die)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag") {

		if (playEffectDistance_) {
			//ヒットエフェクトの再生
			effectHit_ = gsPlayEffect(Effect_Hit, &myPos_);
			gsPlaySE(SE_Damage1);
		}

		//ダメージを受け取る関数
		damage_ = static_cast<BasicAttackCollider*>(&other)->GetAttackValue() - defensive_;

		if (damage_ <= 0) {
			damage_ = 0;
		}

		//体力を減らす
		health_ -= damage_;

		if (health_ <= 0) {

			gsStopEffect(vernierEffect_);

			//KILL数をカウント
			if (other.name() == "BeamRifleBullet") {
				world_->gameData()->setBeamRifleKillCounter(1);
			}
			else if (other.name() == "BeamMagnumBullet") {
				world_->gameData()->setBeamMagnumKillCounter(1);
			}
			else if (other.name() == "BazookaBullet") {
				world_->gameData()->setBazookaKillCounter(1);
			}
			else if (other.name() == "AllRangeBullet") {
				world_->gameData()->setAllRangeUnitKillCounter(1);
			}

			//武器ごとのプレイヤーのスキルポイント量を変える
			if (other.name() != "AllRangeBullet") {
				switch (Weapon_)
				{
				case HBM::Weapon::BeamSaber:
					player_->player_state()->addExSkillPoint(50);
					break;
				case HBM::Weapon::Gatling:
					player_->player_state()->addExSkillPoint(30);
					break;
				case HBM::Weapon::BeamRifle:
					player_->player_state()->addExSkillPoint(50);
					break;
				case HBM::Weapon::Sniper:
					player_->player_state()->addExSkillPoint(100);
					break;
				}
			}

			//斬撃
			if (Weapon_ == HBM::Weapon::BeamSaber) {
				change_state(State::Die, Motion_Die_SaberEarth, false);
			}
			//銃
			else {
				change_state(State::Die, Motion_Die_GunEarth, false);
			}
		}

		else {

			//現在のステータスを保持
			frontState_ = state_;

			//弾の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;

			//ダメージ状態に遷移する
			//斬撃
			if (Weapon_ == HBM::Weapon::BeamSaber) {
				change_state(State::Damage, Motion_Damage1_SaberEarth, false);
			}
			//銃
			else {
				change_state(State::Damage, Motion_Damage_GunEarth, false);
			}
		}
		return;
	}
	//プレイヤーまたは敵に衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

//AI側からのステータス更新
void HBM::changeState(int state) {

	switch (state)
	{
	case 1:
		change_state(State::Idle, 0);
		break;
	case 2:
		change_state(State::Move, Motion_WarkF_A);
		break;
	case 3:
		change_state(State::Attack, 0);
		break;
	case 4:
		change_state(State::Slashing, 0);
		break;
	case 5:
		change_state(State::FeintSlashing, 0);
		break;
	case 6:
		change_state(State::Damage, 0);
		break;
	case 7:
		change_state(State::RunAway, 0);
		break;
	case 8:
		change_state(State::Die, 0);
		break;
	}
}

//AI側に現在のステータスを返す
int HBM::stateNow() {

	switch (state_)
	{
	case HBM::State::Idle:
		return 1;
		break;
	case HBM::State::Move:
		return 2;
		break;
	case HBM::State::Attack:
		return 3;
		break;
	case HBM::State::Slashing:
		return 4;
		break;
	case HBM::State::FeintSlashing:
		return 5;
		break;
	case HBM::State::Damage:
		return 6;
		break;
	case HBM::State::RunAway:
		return 7;
		break;
	case HBM::State::Die:
		return 8;
		break;
	}
}

//目標地点
void HBM::attackPoint(GSvector3 pos) {
	destination_ = pos;
}

//距離に応じてバーニアエフェクトを停止する
void HBM::vernierstop() {

	//停止
	if (!playEffectDistance_ && gsExistsEffect(vernierEffect_)) {
		gsStopEffect(vernierEffect_);
	}

	bool test = gsExistsEffect(vernierEffect_);

	//再生
	if (playEffectDistance_ && !test && state_ == State::Move) {
		vernierEffect_ = gsPlayEffect(Effect_VernierBL, &myPos_);
	}
}

//ステータスの更新
void HBM::update_state(float delta_time) {

	//各状態に分岐する
	switch (state_)
	{
	case HBM::State::Idle:
		idle(delta_time);
		break;
	case HBM::State::Move:
		move(delta_time);
		break;
	case HBM::State::Attack:
		attack(delta_time);
		break;
	case HBM::State::Slashing:
		SlashingAttack(delta_time);
		break;
	case HBM::State::FeintSlashing:
		SlashingFeint(delta_time);
		break;
	case HBM::State::Damage:
		damage(delta_time);
		break;
	case HBM::State::RunAway:
		runaway(delta_time);
		break;
	case HBM::State::Die:
		Die(delta_time);
		break;
	}
	stateTimer_ += delta_time;
}

void HBM::change_state(State state, GSuint motion, bool loop) {
	//モーション番号の更新
	motion_ = motion;
	//モーションのループ指定
	motionLoop_ = loop;
	//状態の更新
	state_ = state;
	stateTimer_ = 0.f;
}

//アイドル
void HBM::idle(float delta_time) {
	//何もなければ、アイドル状態のまま
	if (Weapon_ == HBM::Weapon::BeamSaber)change_state(State::Idle, Motion_Idle_SaberEarth);
	else change_state(State::Idle, Motion_Idle_G);
}

//移動
void HBM::move(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (turnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -turnAngle_, turnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);
	//移動
	velocity_ = (destination_ - transform_.position()).normalized();

	transform_.translate(velocity_ * runSpeed_ * delta_time, GStransform::Space::World);

	//目標地点に到達したら攻撃開始
	if (target_distance() <= 1.5f) {
		change_state(State::Attack, 0);
		gsStopEffect(vernierEffect_);
	}
}

//攻撃
void HBM::attack(float delta_time) {

	//プレイヤーの方向を向かせる
	faceThePlayer(delta_time);

	//武器によって攻撃時の処理を変える
	switch (Weapon_)
	{
	case HBM::Weapon::BeamSaber:
		SlashingMove(delta_time);
		break;
	case HBM::Weapon::Gatling:
		Gatring(delta_time);
		break;
	case HBM::Weapon::BeamRifle:
		BeamLifre(delta_time);
		break;
	case HBM::Weapon::Sniper:
		Snaiper(delta_time);
		break;
	}
}

//ビームサーベル装備中の移動
void HBM::SlashingMove(float delta_time) {

	//次の移動までの時間
	attackMoveTimer_ -= delta_time;

	//移動
	if (!aiAttackFrag_) {

		//時間がたっていたら移動方向を変える
		if (attackMoveTimer_ <= 0) {
			sign_ = sign();
			attackMoveTimer_ = gsRand(moveRandSabel_.x, moveRandSabel_.y);
		}
		transform_.translate(transform_.position().right() * sign_ * walkSpeed_ * delta_time);

		//自身のフォワードと方向ベクトルの角度差を符号付きで取得
		float angle = GSvector3::signedAngle(transform_.forward(), attackMovePoint_.normalized());
		//角度に応じてアニメーションを変える
		if (angle >= -45 && angle <= 45) {
			motion_ = Motion_WarkF_G;
		}
		else if (angle > 45 && angle <= 135) {
			motion_ = Motion_WarkR_G;
		}
		else if (angle < -45 && angle >= -135) {
			motion_ = Motion_WarkL_G;
		}
		else {
			motion_ = Motion_WarkB_G;
		}
	}

	//攻撃に向けた動き
	if (aiAttackFrag_) {

		if (!fryTrigger_ && !aiAfterAttackFrag_) {
			//プレイヤーが浮いている可能性があるので重力処理を行わない
			fryTrigger_ = true;
			vernierEffect_ = gsPlayEffect(Effect_VernierBL, &myPos_);
		}

		//攻撃命令を下げる
		aiAttackFrag_ = false;

		//プレイヤーに向かう方向ベクトル
		GSvector3 playerto = playerPos_ - myPos_;

		//前進
		transform_.translate(playerto.normalized() * runSpeed_ * delta_time, GStransform::Space::World);

		if (player_distance() <= 5) {

			//ランダムでフェイントか攻撃かを選ぶ
			int num = gsRand(1, 2);
			if (num == 1) {
				//攻撃
				change_state(State::Slashing, Motion_WarkF_A);
			}
			else {
				//フェイント
				change_state(State::FeintSlashing, Motion_WarkF_A);
			}
		}
	}
}

//ビームサーベルで攻撃
void HBM::SlashingAttack(float delta_time) {

	//一定距離近づいたら攻撃
	if (player_distance() <= 2 && !afterSlashFrag_) {
		generate_bullet();
		afterSlashFrag_ = true;
	}

	//攻撃後又は時間制限になったなら後方に移動する
	if (afterSlashFrag_ || fnishSlashTimer_ <= 0) {

		//時間０の時にフラグが上がっていないため
		afterSlashFrag_ = true;

		if (fryTrigger_) {
			//重力処理
			fryTrigger_ = false;
		}

		GSvector3 moveto = -transform_.forward();

		moveto.y += velocity_.y;

		//後ろに下がる
		transform_.translate(moveto * runSpeed_ * delta_time, GStransform::Space::World);

		//プレイヤーと一定距離離れたら
		if (isGround_) {
			//攻撃移動ステータスに移行
			fnishSlashTimer_ = fnishSlashTimeAssignment_;
			aiAfterAttackFrag_ = true;
			afterSlashFrag_ = false;
			gsStopEffect(vernierEffect_);
			change_state(State::Attack, Motion_Attack_GunEarth);
		}
	}
	//攻撃前ならプレイヤーに向かって前進
	else {
		//プレイヤーに向かう方向ベクトル
		GSvector3 playerto = playerPos_ - myPos_;
		//前進
		transform_.translate(playerto.normalized() * runSpeed_ * delta_time, GStransform::Space::World);
	}
}

//ビームサーベル装備時のフェイント
void HBM::SlashingFeint(float delta_time) {

	fryTrigger_ = false;

	GSvector3 moveto = -transform_.forward();

	moveto.y += velocity_.y;

	//後ろに下がる
	transform_.translate(moveto * runSpeed_ * delta_time, GStransform::Space::World);

	//一定距離離れたら
	if (isGround_) {
		//攻撃移動ステータスに移行
		aiAfterAttackFrag_ = true;
		gsStopEffect(vernierEffect_);
		change_state(State::Attack, Motion_Attack_GunEarth);
	}
}

//AI側が攻撃命令をする
void HBM::setattackfrag(bool frag) {
	aiAttackFrag_ = frag;
}

//攻撃命令フラグ取得
bool HBM::attackfrag()const {
	return aiAttackFrag_;
}

//AIが攻撃後のフラグを変更
void HBM::setafterattackfrag(bool frag) {
	aiAfterAttackFrag_ = frag;
}

//AI に攻撃後かどうかを知らせる
bool HBM::afterattackfrag() const {
	return aiAfterAttackFrag_;
}

//武器指定して弾込め
void HBM::SetBullet(HBM::Weapon weapon) {
	if (weapon == HBM::Weapon::Gatling) gtringBullet_ = assignmentGtringBullet_;
	if (weapon == HBM::Weapon::BeamRifle)beamLifleBullet_ = assignmentbeamLifleBullet_;
}

//ガトリングで攻撃
void HBM::Gatring(float delta_time) {

	attackMoveTimer_ -= delta_time;

	//部隊の中心から一定距離離れているか
	centerDistance_ = GSvector3::distance(myPos_, destination_);

	if (centerDistance_ > 4) {
		moveCenterFrag_ = true;
		attackMovePoint_ = destination_ - myPos_;
	}
	else if (centerDistance_ <= 1) {
		moveCenterFrag_ = false;
	}

	//部隊の中心から一定距離離れたもしくは一定時間経ったら
	if (attackMoveTimer_ <= 0.0f && !moveCenterFrag_) {
		//ランダム時間初期化 3~5秒
		attackMoveTimer_ = gsRand(moveRandGatling_.x, moveRandGatling_.y) * 60.0f;
		attackMovePoint_ = GSvector3{ (float)gsRand(-1,1),0,(float)gsRand(-1,1) };
	}
	//移動
	transform_.translate(attackMovePoint_.normalized() * gatringWalkSpeed_ * delta_time, GStransform::Space::World);

	//自身のフォワードと方向ベクトルの角度差を符号付きで取得
	float angle = GSvector3::signedAngle(transform_.forward(), attackMovePoint_.normalized());

	//移動量があるかどうか
	if (attackMovePoint_ == GSvector3::zero()) {

		//射撃時でアニメーションを変える
		if (aiAttackFrag_)motion_ = Motion_Attack_GunEarth;
		else motion_ = Motion_Idle_G;
	}
	else {
		//角度に応じてアニメーションを変える
		if (angle >= -45 && angle <= 45) {
			motion_ = Motion_WarkF_G;
		}
		else if (angle > 45 && angle <= 135) {
			motion_ = Motion_WarkR_G;
		}
		else if (angle < -45 && angle >= -135) {
			motion_ = Motion_WarkL_G;
		}
		else {
			motion_ = Motion_WarkB_G;
		}
	}

	//弾発射プロセス
	if (aiAttackFrag_) {
		//攻撃時間		
		attackTimer_ -= delta_time;

		if (attackTimer_ <= 0) {
			generate_bullet();
			attackTimer_ = 10.0f;
			gtringBullet_--;
		}
		if (gtringBullet_ <= 0) {
			aiAttackFrag_ = false;
			aiAfterAttackFrag_ = true;
		}
	}
}

//ビームライフルで攻撃
void HBM::BeamLifre(float delta_time) {
	attackMoveTimer_ -= delta_time;

	//部隊の中心から一定距離離れているか
	centerDistance_ = GSvector3::distance(myPos_, destination_);

	if (centerDistance_ > 4) {
		moveCenterFrag_ = true;
		attackMovePoint_ = destination_ - myPos_;
	}
	else if (centerDistance_ <= 1) {
		moveCenterFrag_ = false;
	}

	//部隊の中心から一定距離離れたもしくは一定時間経ったら
	if (attackMoveTimer_ <= 0.0f && !moveCenterFrag_) {
		//ランダム時間初期化 3~5秒
		attackMoveTimer_ = gsRand(moveRandBeamRifle_.x, moveRandBeamRifle_.y) * 60.0f;
		attackMovePoint_ = GSvector3{ (float)gsRand(-1,1),0,(float)gsRand(-1,1) };
	}

	//移動
	transform_.translate(attackMovePoint_.normalized() * BeamLifleWalkSpeed_ * delta_time, GStransform::Space::World);

	//自身のフォワードと方向ベクトルの角度差を符号付きで取得
	float angle = GSvector3::signedAngle(transform_.forward(), attackMovePoint_.normalized());

	//移動量があるかどうか
	if (attackMovePoint_ == GSvector3::zero()) {

		//射撃時でアニメーションを変える
		if (aiAttackFrag_)motion_ = Motion_Attack_GunEarth;
		else motion_ = Motion_Idle_G;
	}
	else {
		//角度に応じてアニメーションを変える
		if (angle >= -45 && angle <= 45) {
			motion_ = Motion_WarkF_G;
		}
		else if (angle > 45 && angle <= 135) {
			motion_ = Motion_WarkR_G;
		}
		else if (angle < -45 && angle >= -135) {
			motion_ = Motion_WarkL_G;
		}
		else {
			motion_ = Motion_WarkB_G;
		}
	}

	//攻撃命令が出されたら
	if (aiAttackFrag_) {
		attackTimer_ -= delta_time;

		if (attackTimer_ <= 0) {
			generate_bullet();
			attackTimer_ = 20.0f;
			beamLifleBullet_--;
		}
		if (beamLifleBullet_ <= 0) {
			aiAttackFrag_ = false;
			aiAfterAttackFrag_ = true;
		}
	}
}

//スナイパーで攻撃
void HBM::Snaiper(float delta_time) {

	//攻撃命令が下ったら
	if (aiAttackFrag_) {
		//攻撃命令を下げる
		aiAttackFrag_ = false;
		//弾生成
		generate_bullet();
		//AIに知らせる攻撃後のフラグ
		aiAfterAttackFrag_ = true;
	}
}

//ダメージ
void HBM::damage(float delta_time) {

	//ノックバックする
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	velocity_ -= GSvector3{ velocity_.x,0.f,velocity_.z }*0.5f * delta_time;
	//ダメージを受ける前のステータスにする
	change_state(frontState_, Motion_WarkF_A);
}

//退却
void HBM::runaway(float delta_time) {

	//撤退フラグを上げる
	runAwayFrag_ = true;

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (turnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -turnAngle_, turnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);
	//前進する（ローカル座標）

	GSvector3 pointv = destination_ - transform_.position();
	transform_.translate(pointv.normalized() * delta_time * runSpeed_, GStransform::Space::World);

	//目標地点に到達したら死亡状態にする
	if (target_distance() <= 1.5f) {

		tag_ = "DieEnemyTag";
		change_state(State::Die, 0);
		//バーニアエフェクト停止
		gsStopEffect(vernierEffect_);
	}
}

//死
void HBM::Die(float delta_time) {

	//撤退による死でない時
	if (!runAwayFrag_) {
		//モーションし終えたらメッシュを描画しない
		if (stateTimer_ >= dieTimer_) {

			//爆発エフェクト再生していなかったら
			if (!playExplosionEffect_) {

				if (playEffectDistance_)gsPlaySE(SE_DieExplosion);

				playExplosionEffect_ = true;
				//爆発エフェクトをその場で再生
				effectExplosionL_ = gsPlayEffect(Effect_ExplosionL, &myPos_);
				drawMeshFrag_ = false;
			}

			//爆発エフェクトの再生が終了したらタグ変更
			if (!gsExistsEffect(effectExplosionL_))	tag_ = "DieEnemyTag";
		}
	}
}

//弾生成
void HBM::generate_bullet() {

	GSvector3 position = myPos_ + transform_.forward();
	GSvector3 velocity;
	position.y += 1.0f;

	if (Weapon_ == HBM::Weapon::Gatling) {
		//ガトリングの弾を拡散させる
		velocity = ((playerPos_ - position) + GSvector3{ (float)gsRand(-2,2), (float)gsRand(-2,2), (float)gsRand(-2,2) }).normalized() * 0.5f;
	}
	else {
		velocity = (playerPos_ - position).normalized() * 0.5f;
	}

	switch (Weapon_)
	{
	case HBM::Weapon::BeamSaber:
		world_->add_actor(new EnemyAttackRange{ world_,position,GSvector3().zero(),attackValue_ });
		break;
	case HBM::Weapon::Gatling:
		world_->add_actor(new GatlingBullet{ world_,position,velocity,attackValue_ });
		break;
	case HBM::Weapon::BeamRifle:
		world_->add_actor(new UnderBossBeamRifleBullet{ world_,position,velocity,attackValue_ });
		break;
	case HBM::Weapon::Sniper:
		world_->add_actor(new SniperBullet{ world_,position,velocity * 2 ,attackValue_ });
		break;
	}
}

//移動時に目標地点との角度を符号付きで返す
float HBM::target_signed_angle() {

	//自身と目標地点の座標の方向ベクトルを求める
	GSvector3 to_target = destination_ - transform_.position();
	//自身の前ベクトルを求める
	GSvector3 forward = transform_.forward();

	//ベクトルのy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;

	return GSvector3::signedAngle(forward, to_target);
}

float HBM::target_signed_angle_fire() {

	if (player_ == nullptr)return 0.0f;

	//自身とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_target = playerPos_ - transform_.position();
	//自身の前ベクトルを求める
	GSvector3 forward = transform_.forward();

	//ベクトルのy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;

	return GSvector3::signedAngle(forward, to_target);
}

//自身と目標との間
float HBM::target_distance() {
	return GSvector3::distance(destination_, transform_.position());
}

//プレイヤーとの距離を出す
float HBM::player_distance() {
	return GSvector3::distance(playerPos_, transform_.position());
}

//プレイヤーの方向を向かせる
void HBM::faceThePlayer(float delta_time) {

	float angle;

	//ステータスに応じて向く方向を変える
	if (HBM::state_ == State::Attack)angle = target_signed_angle_fire();
	else angle = target_signed_angle();

	if (std::abs(angle) > (turnAngle_ * delta_time))angle = CLAMP(angle, -turnAngle_, turnAngle_) * delta_time;

	transform_.rotate(0.f, angle, 0.f);
}

//符号付きの数字を返す
int HBM::sign() {
	int num = gsRand(-1, 1);

	if (num == 1 || num == -1)return num;
	return sign();
}

void HBM::collide_field() {

	//壁との衝突判定（球体との判定）
	GSvector3 center;//衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		//y座標は変更しない
		center.y = transform_.position().y;
		//補正後の座標に変換する
		transform_.position(center);
	}
	//地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.f,-footOffset_,0.f };
	GSvector3 intersect;//地面との交差
	if (world_->field()->collide(line, &intersect)) {
		//交差した点からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.f;
		isGround_ = true;
	}
	else {
		isGround_ = false;
	}
}

void HBM::collide_actor(Actor& other) {

	//y座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.y = 0.f;
	GSvector3 target = other.transform().position();
	target.y = 0.f;
	//相手との距離
	float distance = GSvector3::distance(position, target);
	//衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius + other.collider().radius;
	//衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	//重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	collide_field();
}