#include "Boss.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "Common/GameData.h"
#include "Collision/Ray.h"
#include "BattleShip/EnemyShip.h"
#include "Collision/BasicAttackCollider.h"
#include "BossGun/Missile.h"
#include "BossGun/BossDamageRange.h"
#include "BossGun/BossBeamLifle.h"
#include "GSeffect.h"

//アニメーション
enum {

	//地上アイドル
	Motion_Idle_Ground = 0,
	//地上移動
	Motion_WalkF_Ground = 1,
	Motion_WalkB_Ground = 2,
	Motion_WalkL_Ground = 3,
	Motion_WalkR_Ground = 4,
	//地上タックル
	Motion_Tackle_Ground = 5,
	//地上薙ぎ払い
	Motion_Cleaver_Ground = 6,
	//地上ビームライフル
	Motion_Fire_Ground = 7,
	//地上ジャンプ
	Motion_Jump_Ground = 8,
	//地上空中
	Motion_Air_Ground = 9,
	//地上たたきつけ
	Motion_Landing_Ground = 10,
	//宙に浮く
	Motion_SkyUp_Ground = 11,
	//空中アイドル
	Motion_Idle_Air = 12,
	//空中移動
	Motion_WalkF_Air = 13,
	Motion_WalkB_Air = 14,
	Motion_WalkL_Air = 15,
	Motion_WalkR_Air = 16,
	//空中タックル
	Motion_Tackle_Air = 17,
	//空中ビームライフル
	Motion_Fire_Air = 18,
	//空中死亡
	Motion_Die_Air = 19,
};

//コンストラクタ
Boss::Boss(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Boss,Mesh_Boss ,Mesh_Boss,Motion_Idle_Air,true }
{
	world_ = world;
	tag_ = "BossTag";
	name_ = "Boss";
	collider_ = BoundingSphere{ bossRadius_,GSvector3{0.f,bossHeight_,0.f} };
	transform_.position(position);
	mesh_.Transform(transform_.localToWorldMatrix());

	//ボス生成
	bossstate_ = new BossState();

	//プレイヤーの取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//自陣の戦艦を取得
	enemyShip_ = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));

	//ステータス初期化
	bossstate_->initialize_state_();
}

Boss::~Boss() {
	delete bossstate_;
}

void Boss::update(float delta_time) {

	//移動速度
	walkSpeed_ = bossstate_->moveSpeed();

	//状態の更新
	update_state(delta_time);

	//重力処理
	if (isFly_) velocity_.y = 0.0f;
	else velocity_.y += gravity_ * delta_time;
	transform_.translate(0.f, velocity_.y, 0.0f);

	//フィールドとの当たり判定
	collide_field();

	//メッシュのモーションを更新
	mesh_.ChangeMotion(motion_, motionLoop_);

	mesh_.Update(delta_time);

	//ワールド変換行列を設定
	mesh_.Transform(transform_.localToWorldMatrix());

	//自身の座標を取得
	myPos_ = transform_.position();

	//プレイヤーの座標を取得
	playerPos_ = player_->transform().position();

	//無敵時間
	if (damageFrag_) {
		invincibleTimer_ -= delta_time;
		if (invincibleTimer_ <= 0) {
			invincibleTimer_ = assignmentInvincibleTimer_;
			damageFrag_ = false;
			meshAlpha_ = nomalAlpha_;
		}
	}
}

//描画
void Boss::draw() const {

	if (!dieTrigger_) {

		//ダメージ受けたときに半透明にする
		float transparency = gsGetDitheredTransparency();
		GScolor current_color;
		glGetFloatv(GL_CURRENT_COLOR, current_color);
		GScolor current_secondary_color;
		glGetFloatv(GL_CURRENT_SECONDARY_COLOR, current_secondary_color);
		gsSetDitheredTransparency(meshAlpha_);
		//メッシュの描画
		mesh_.Draw();
		//武器を描画
		gsSetDitheredTransparency(transparency);
		glColor4fv(current_color);
		glSecondaryColor3fv(current_secondary_color);
	}
}

//当たり判定
void Boss::react(Actor& other) {

	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Die || invincible_)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag" && !damageFrag_) {

		//ダメージを受け取る関数
		damageValue_ = static_cast<BasicAttackCollider*>(&other)->GetAttackValue() - bossstate_->defense();

		if (damageValue_ <= 0) {
			damageValue_ = 0;
		}

		//体力を減らす
		bossstate_->AddHP(-damageValue_);

		//ダメージSE
		gsPlaySE(SE_Damage1);

		if (bossstate_->HP() <= 0) {
			//爆発SE
			gsPlaySE(SE_BossDieExplosion);
			//爆発エフェクト再生
			effectExplosion_ = gsPlayEffect(Effect_ExplosionL, &myPos_);
			change_state(State::Die, Motion_Die_Air, false);
		}
		else {

			GSvector3 otherVelocity = other.velocity().getNormalized();
			otherVelocity.y = 0;

			//弾の進行方向にノックバックする移動量を求める
			velocity_ = otherVelocity * 0.5f;

			//ビームライフルのクールタイム初期化
			beamFireCoolTime_ = assignmentBeamFireCoolTime_;

			damageFrag_ = true;
			meshAlpha_ = damageAlpha_;

			//ダメージ状態に遷移する
			state_ = Boss::State::Damage;
		}
		return;
	}
	//プレイヤーまたは敵に衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

//死亡したかどうかをほかに知らせる
bool Boss::die_trigger() const {
	return dieTrigger_;
}

//ボスステータスを返す
BossState* Boss::boss_state() const {
	return bossstate_;
}

//ステータス更新
void Boss::update_state(float delta_time) {

	switch (state_)
	{
	case Boss::FirstMove:
		first_move(delta_time);
		break;
	case Boss::AttackMove:
		attack_move(delta_time);
		break;
	case Boss::Cleaver:
		cleaver(delta_time);
		break;
	case Boss::FireBullet:
		bullet_fire(delta_time);
		break;
	case Boss::Damage:
		damage(delta_time);
		break;
	case Boss::Die:
		die(delta_time);
		break;
	}
	stateTimer_ += delta_time;
}

//ステータス変更
void Boss::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motionLoop_ = loop;
	state_ = state;
	stateTimer_ = 0.f;
}

//初めの行動
void Boss::first_move(float delta_time) {

	// 戦艦の前方10m地点をターゲットに設定
	targetPoint_ = enemyShip_->transform().position() + firstMovePointX_;

	// 目標地点への移動ベクトルを計算
	GSvector3 moveDir = (targetPoint_ - transform_.position()).normalized();

	// 移動速度を設定
	velocity_ = moveDir * walkSpeed_;
	// 移動処理 (重力なし)
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

	//目標地点の方向に向かせる
	face_the_target(moveDir, delta_time);

	// 目標地点にある程度近づいたらステート変更
	if (GSvector3::distance(transform_.position(), targetPoint_) <= firstMoveFinishDistance_) {
		// 無敵解除
		invincible_ = false;

		//飛ばないようにする
		isFly_ = false;
		// 状態を移動攻撃に変更
		change_state(State::AttackMove, Motion_Idle_Ground, true);
		targetPoint_ = GSvector3::zero();
		return;
	}
}

//攻撃
void Boss::attack_move(float delta_time) {

	//プレイヤーの方向を向かせる
	face_the_target(playerPos_, delta_time);

	//移動
	if (!randMoveFrag_ && groundFrag_) {
		//ランダムな座標を設定
		targetPoint_ = player_->transform().position() + GSvector3{ (float)gsRand(-randPos_,randPos_),0.0f,(float)gsRand(-randPos_,randPos_) };
		targetPoint_.y = transform_.position().y;
		targetPoint_.x = CLAMP(targetPoint_.x, clampPosX_.x, clampPosX_.y);
		targetPoint_.z = CLAMP(targetPoint_.z, clampPosZ_.x, clampPosZ_.y);
		randMoveFrag_ = true;
	}

	//目標地点付近になったら新たな目標地点を設定
	if (GSvector3::distance(targetPoint_, transform_.position()) <= updateRandPos_) {
		randMoveFrag_ = false;
	}

	//目標地点から自身の座標を引いたもの
	GSvector3 targetpos = targetPoint_ - transform_.position();

	//目標地点の方向に移動を開始する
	transform_.translate(targetpos.normalized() * walkSpeed_ * delta_time, GStransform::Space::World);

	//薙ぎ払い
	if (GSvector3::distance(transform_.position(), playerPos_) <= cleaverDistance_) {
		change_state(State::Cleaver, Motion_Cleaver_Ground);
		return;
	}

	//ビームライフルのクールタイム
	beamFireCoolTime_ -= delta_time;
	//ミサイルのクールタイム
	missileCoolTime_ -= delta_time;

	//射撃
	if (beamFireCoolTime_ <= 0.0f) {

		if (missileCoolTime_ <= 0.0f) {
			//ランダムでミサイルを発射
			int randmissie = gsRand(0, 1);
			if (randmissie == 1) fire_missile(delta_time);
		}

		//ビームライフルステータスに移行
		change_state(State::FireBullet, Motion_Fire_Ground);
		beamFireTime_ = assignmentBeamFireTime_;
		return;
	}
}

//薙ぎ払い
void Boss::cleaver(float delta_time) {

	if (!cleaverTrigger_) {
		makeDamageRangePos_ = transform_.position() + transform_.forward() * makeCleaverPosOffset_;
		makeDamageRangePos_.y += bossHeight_;
		world_->add_actor(new BossDamageRange{ world_,makeDamageRangePos_,GSvector3::zero(),slashAttackValue_,BossDamageRange::EffectState::Slash ,slashRadius_ });
		cleaverTrigger_ = true;
	}
	//ステータス変更
	if (stateTimer_ >= mesh_.MotionEndTime()) {
		change_state(Boss::AttackMove, Motion_Idle_Ground);
		cleaverTrigger_ = false;
	}
}

//ダメージ
void Boss::damage(float delta_time) {
	//ヒットエフェクト再生
	effectHit_ = gsPlayEffect(Effect_Hit, &myPos_);

	//アニメーション再生後移動攻撃にステータス変更
	if (gsExistsEffect(effectHit_)) {
		change_state(State::AttackMove, Motion_Idle_Ground);
	}
}

//死亡
void Boss::die(float delta_time) {

	//前のエフェクトが再生し終えたら新しいものを再生
	if (!dieTrigger_) {
		//ランダムな場所に出す
		GSvector3 randpos = GSvector3{ (float)gsRand(-randEffectPos_,randEffectPos_),(float)gsRand(-randEffectPos_,randEffectPos_),(float)gsRand(-randEffectPos_,randEffectPos_) }
		+ transform_.position();
		effectExplosion_ = gsPlayEffect(Effect_ExplosionL, &randpos);
	}

	//アニメーションが終わったら死亡する
	if (stateTimer_ >= mesh_.MotionEndTime() && !dieTrigger_) {
		//ゲーム側に死亡を知らせる
		world_->gameData()->setBossDie(true);
		dieTrigger_ = true;
	}
}

//ビームライフル射撃
void Boss::bullet_fire(float delta_time) {

	//このステータスの時間
	beamFireTime_ -= delta_time;
	//次の射撃までの時間
	fireInterval_ -= delta_time;

	//射撃タイミングになったら
	if (fireInterval_ <= 0.0f) {

		makeBeamRiflePos_ = transform_.position();
		makeBeamRiflePos_.y += 4;

		playerPos_ = player_->transform().position();
		playerPos_.y += 1.0f;

		beamRifleVelocity_ = playerPos_ - makeBeamRiflePos_;
		world_->add_actor(new BossBeamLifle{ world_,makeBeamRiflePos_  , beamRifleVelocity_.normalized() ,beamAttackValue_ });

		fireInterval_ = assignmentFireInterval_;
	}

	//弾生成後移動攻撃に移行
	if (beamFireTime_ <= 0.0f) {
		change_state(State::AttackMove, Motion_Idle_Ground);
		beamFireCoolTime_ = assignmentBeamFireCoolTime_;
	}
}

//ミサイル生成
void Boss::fire_missile(float delta_time) {

	//ミサイル生成処理
	for (int i = 0; i < makeMissileNum_; i++) {

		missileMakePoint_ = transform_.position();

		//高さ調整
		missileMakePoint_.y += bossHeight_;

		//奥行きの調整
		missileMakePoint_ -= transform_.forward().normalized() * 2;
		world_->add_actor(new Missile{ world_,missileMakePoint_,transform_.up().normalized(),missileAttackValue_ });

	}
	//クールタイムの代入
	missileCoolTime_ = assignmentMissileCoolTime_;
}

//ターゲットの方向を向かせる
void Boss::face_the_target(GSvector3 target, float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle(target);

	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (turnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -turnAngle_, turnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);
}

//ターゲットとの角度を符号付きで返す
float Boss::target_signed_angle(GSvector3 target) {

	//プレイヤーと自身の座標の方向ベクトル
	GSvector3 to_target = target - transform_.position();

	GSvector3 forward = transform_.forward();

	to_target.y = 0;
	forward.y = 0;

	return GSvector3::signedAngle(forward, to_target);
}

//地面との当たり判定
void Boss::collide_field() {
	//壁との衝突判定（球体との判定）
	GSvector3 center;//衝突後の球体の中心位置
	if (world_->field()->collide(collider(), &center)) {
		//ｙ座標は変更しない
		center.y = transform_.position().y;
		//補正後の座標に変更する
		transform_.position(center);
	}
	//地面との衝突判定（線分との交点）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.f,-footOffSet_,0.f };
	GSvector3 intersect;//地面との交点
	if (world_->field()->collide(line, &intersect)) {
		groundFrag_ = true;
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.f;
	}
}

//ほかのアクターとの当たり
void Boss::collide_actor(Actor& other) {

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
	//重なっている部分の半分の距離だけ離れる移動値を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	collide_field();
}