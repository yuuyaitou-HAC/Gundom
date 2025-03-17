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

//ボスの高さ
const float BossHeight_{ 4.f };

//衝突判定用の半径
const float BossRadius_{ 3.5f };

//振り返るときの速度
const float TurnAngle_{ 2.5f };

//重力
const float Gravity_{ -0.016f };

//足元のオフセット
const float FootOffset_{ 0.1f };

//コンストラクタ
Boss::Boss(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Boss,Mesh_Boss ,Mesh_Boss,Motion_Idle_Air,true },
	motion_{ Motion_Idle_Air },
	state_{ State::FirstMove },
	form_{ Form::first } {

	world_ = world;
	tag_ = "BossTag";
	name_ = "Boss";
	collider_ = BoundingSphere{ BossRadius_,GSvector3{0.f,BossHeight_,0.f} };
	transform_.position(position);
	mesh_.Transform(transform_.localToWorldMatrix());

	bossstate_ = new BossState();

	//プレイヤーの取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//自陣の戦艦を取得
	enemyship_ = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));

	//ステータス初期化
	bossstate_->initialize_state_();

	//飛んでいる状態にする
	isfry_ = true;
	//無敵状態にする
	invincible_ = true;
}

Boss::~Boss() {
	delete bossstate_;
}

void Boss::update(float delta_time) {

	//輪の透明度　影は透明度に関係なく出る
	//if (gsGetKeyState(GKEY_UPARROW))test_ += delta_time * 0.01;
	//else if (gsGetKeyState(GKEY_DOWNARROW))	test_ -= delta_time * 0.01;
	//test_ = CLAMP(test_, 0.0f, 1.0f);

	//移動速度
	walkSpeed_ = bossstate_->moveSpeed();

	//状態の更新
	update_state(delta_time);

	//重力処理
	if (isfry_) velocity_.y = 0.0f;
	else velocity_.y += Gravity_ * delta_time;
	transform_.translate(0.f, velocity_.y, 0.0f);

	//フィールドとの当たり判定
	collide_field();

	//メッシュのモーションを更新
	mesh_.ChangeMotion(motion_, motion_Loop_);

	mesh_.Update(delta_time);

	//ワールド変換行列を設定
	mesh_.Transform(transform_.localToWorldMatrix());

	//自身の座標を取得
	myPos_ = transform_.position();

	//プレイヤーの座標を取得
	playerPos_ = player_->transform().position();

	if (gsGetKeyTrigger(GKEY_P)) {
		missileMakePoint_ = transform_.position();

		//高さ調整
		missileMakePoint_.y += BossHeight_;

		//奥行きの調整
		missileMakePoint_ -= transform_.forward().normalized() * 2;
		world_->add_actor(new Missile{ world_,missileMakePoint_,transform_.up().normalized(),bossstate_->attack() });
	}

	if (gsGetKeyTrigger(GKEY_O)) {
		//砂埃
		makeDamageRangePos_ = transform_.position();
		makeDamageRangePos_.y += BossHeight_;

		world_->add_actor(new BossDamageRange{ world_,makeDamageRangePos_,GSvector3::zero(),bossstate_->attack(),1,4.0f });

	}

	//弾生成
	if (gsGetKeyTrigger(GKEY_U)) {


	}

	if (gsGetKeyTrigger(GKEY_UPARROW)) {
		radiusTest_++;
	}
	else if (gsGetKeyTrigger(GKEY_DOWNARROW)) {
		radiusTest_--;
	}
}

void Boss::draw() const {

	if (!dieTrigger_) {
		//メッシュを描画
		mesh_.Draw();
		//第二形態時に輪を描画
		if (form_ == Form::second) {
			//金の輪を描画
			glPushMatrix();
			glMultMatrixf(mesh_.BoneMatrices(4));
			glScaled(2, 2, 1);
			glRotated(-0, 1, 0, 0);
			glColor4f(1.0f, 1.0f, 1.0f, test_);
			gsDrawMesh(Mesh_GoldWheel);
			glPopMatrix();
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		}
		collider().draw();
	}

	gsTextPos(100, 100);
	gsDrawText("HP %d", bossstate_->HP());

}

void Boss::draw_gui() const {
	//体力バーの描画

}

void Boss::react(Actor& other) {

	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Die || invincible_)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag") {

		//ダメージを受け取る関数
		damageValue_ = static_cast<BasicAttackCollider*>(&other)->GetAttackValue();

		//体力を減らす
		bossstate_->AddHP(-damageValue_);
		if (bossstate_->HP() <= 0) {
			change_state(State::Die, Motion_Die_Air);
		}
		else {
			//弾の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//ダメージ状態に遷移する
			change_state(State::Damage, 0, false);
		}
		return;
	}
	//プレイヤーまたは敵に衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

BossState* Boss::bossState_() const {
	return bossstate_;
}

void Boss::update_state(float delta_time) {

	switch (state_)
	{
	case Boss::FirstMove:
		farstMove(delta_time);
		break;
	case Boss::Move:
		move(delta_time);
		break;
	case Boss::AttackMove:
		attackmove(delta_time);
		break;
	case Boss::FryAttack:
		fryAttack(delta_time);
		break;
	case Boss::Cleaver:
		cleaver(delta_time);
		break;
	case Boss::FireBullet:
		bulletFire(delta_time);
		break;
	case Boss::Damage:
		damage(delta_time);
		break;
	case Boss::Die:
		die(delta_time);
		break;
	}
	state_timer_ += delta_time;
}

void Boss::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motion_Loop_ = loop;
	state_ = state;
	state_timer_ = 0.f;
}

void Boss::farstMove(float delta_time) {

	// 戦艦の前方10m地点をターゲットに設定
	targetPoint_ = enemyship_->transform().position() + GSvector3{ 50.0f,0.0f,0.0f };

	// 目標地点への移動ベクトルを計算
	GSvector3 moveDir = (targetPoint_ - transform_.position()).normalized();

	faceTheTarget(moveDir, delta_time);

	// 移動速度を設定
	velocity_ = moveDir * walkSpeed_;

	// 移動処理 (重力なし)
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

	// 目標地点にある程度近づいたらステート変更
	if (GSvector3::distance(transform_.position(), targetPoint_) <= walkSpeed_ * delta_time * 1.5f) {
		// 無敵解除
		invincible_ = false;

		//飛ばないようにする
		isfry_ = false;

		// 状態を移動攻撃に変更
		change_state(State::AttackMove, Motion_Idle_Ground, true);
	}
}

void Boss::move(float delta_time) {

}

void Boss::attackmove(float delta_time) {

	//移動

	//プレイヤーの方向を向かせる
	faceTheTarget(playerPos_, delta_time);


	if (GSvector3::distance(transform_.position(), playerPos_) <= 8) {
		//薙ぎ払い
		change_state(State::Cleaver, Motion_Cleaver_Ground);
		return;
	}

	fireCoolTime_ -= delta_time;

	//射撃
	if (fireCoolTime_ <= 0.0f) {
		change_state(State::FireBullet, Motion_Fire_Ground);
		fireTime_ = assignmentFireTime_;
		return;
	}



	//射撃時にクールタイムかランダムか知らんがミサイル撃つかどうか

}

//薙ぎ払い
void Boss::cleaver(float delta_time) {

	if (!cleaverTrigger) {
		makeDamageRangePos_ = transform_.position() + transform_.forward() * 5;
		makeDamageRangePos_.y += BossHeight_;
		world_->add_actor(new BossDamageRange{ world_,makeDamageRangePos_,GSvector3::zero(),bossstate_->attack(),2 ,2.5f });
		cleaverTrigger = true;
	}
	//ステータス変更
	if (state_timer_ >= mesh_.MotionEndTime()) {
		change_state(Boss::AttackMove, Motion_Idle_Ground);
		cleaverTrigger = false;
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

void Boss::die(float delta_time) {

	//爆発エフェクト再生

	//ゲーム側に死んだことを知らせる　死亡フラグを立たせる
	dieTrigger_ = true;
}

void Boss::bulletFire(float delta_time) {

	//このステータスの時間
	fireTime_ -= delta_time;
	//次の射撃までの時間
	fireInterval_ -= delta_time;

	//射撃タイミングになったら
	if (fireInterval_ <= 0.0f) {

		makeBeamLiflePos_ = transform_.position();
		makeBeamLiflePos_.y += 4;

		playerPos_ = player_->transform().position();
		playerPos_.y += 1.0f;

		beamLifleVelocity_ = playerPos_ - makeBeamLiflePos_;
		world_->add_actor(new BossBeamLifle{ world_,makeBeamLiflePos_  , beamLifleVelocity_.normalized() ,bossstate_->attack() });

		fireInterval_ = assignmentFireInterval_;
	}

	//弾生成後移動攻撃に移行
	if (fireTime_ <= 0.0f) {
		change_state(State::AttackMove, Motion_Idle_Ground);
		fireCoolTime_ = assignmentFireCoolTime_;
	}
}

void Boss::missileFire(float delta_time) {

	//ミサイル生成処理

	//クールタイムならここで処理を行う

}

void Boss::fryAttack(float delta_time) {

	//アニメーション再生

	//着地と同時に当たり判定生成

	//アニメーションが終了したら移動攻撃にステータス変更

}

void Boss::faceTheTarget(GSvector3 target, float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle(target);

	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (TurnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -TurnAngle_, TurnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

}

float Boss::target_signed_angle(GSvector3 target) {

	//プレイヤーと自身の座標の方向ベクトル
	GSvector3 to_target = target - transform_.position();

	GSvector3 forward = transform_.forward();

	to_target.y = 0;
	forward.y = 0;

	return GSvector3::signedAngle(forward, to_target);

}

void Boss::generate_bullet() {

	//弾生成

}

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
	line.end = position + GSvector3{ 0.f,-FootOffset_,0.f };
	GSvector3 intersect;//地面との交点
	if (world_->field()->collide(line, &intersect)) {
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.f;
	}
}

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