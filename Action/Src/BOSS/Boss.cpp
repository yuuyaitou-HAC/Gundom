#include "Boss.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "Collision/BasicAttackCollider.h"

//アニメーション
enum {
	//アイドルモーション
	Motion_Idle_GunEarth = 0,
	Motion_Idle_GunAir = 1,
	Motion_Idle_SaberEarth = 2,

	//銃装備時の移動
	Motion_WarkF_GunEarth = 3,
	Motion_WarkB_GunEarth = 4,
	Motion_WarkL_GunEarth = 5,
	Motion_WarkR_GunEarth = 6,

	//銃装備時の空中移動
	Motion_WarkF_GunAir = 7,
	Motion_WarkB_GunAir = 8,
	Motion_WarkL_GunAir = 9,
	Motion_WarkR_GunAir = 10,

	//剣装備時の移動
	Motion_WarkF_SaberEarth = 11,
	Motion_WarkB_SaberEarth = 12,
	Motion_WarkL_SaberEarth = 13,
	Motion_WarkR_SaberEarth = 14,

	//銃装備時の移動攻撃
	Motion_MAttackF_GunEarth = 15,
	Motion_MAttackB_GunEarth = 16,
	Motion_MAttackL_GunEarth = 17,
	Motion_MAttackR_GunEarth = 18,

	//銃装備時の走り
	Motion_RunF_GunEarth = 19,
	Motion_RunB_GunEarth = 20,
	Motion_RunL_GunEarth = 21,
	Motion_RunR_GunEarth = 22,

	//銃装備時の空中高速移動
	Motion_RunF_GunAir = 23,
	Motion_RunL_GunAir = 24,
	Motion_RunR_GunAir = 25,

	//剣装備時の走り
	Motion_RunF_SaberEarth = 26,
	Motion_RunB_SaberEarth = 27,
	Motion_RunL_SaberEarth = 28,
	Motion_RunR_SaberEarth = 29,

	//銃装備時のその場での攻撃
	Motion_Attack_GunEarth = 30,

	//銃装備時のその場での攻撃(空中)
	Motion_Attack1_GunAir = 31,
	Motion_Attack2_GunAir = 32,

	//剣装備時の攻撃(コンボ含む)
	Motion_Attack1_SubarEath = 33,
	Motion_Attack2_SubarEath = 34,
	Motion_Attack3_SubarEath = 35,

	//銃装備時のジャンプ
	Motion_JumpStart_GunEarth = 36,
	Motion_Jump_GunEarth = 37,
	Motion_JumpEnd_GunEarth = 38,

	//剣装備時のジャンプ
	Motion_Jump_SaberEarth = 39,

	//銃装備時の着地
	Motion_Landing_GunEarth = 40,

	//地上にいるときの武器の切り替え(銃)
	Motion_ChangeWepon1_GunEarth = 41,
	Motion_ChangeWepon2_GunEarth = 42,

	//地上にいるときの武器の切り替え(剣)
	Motion_ChangeWepon1_SaberEarth = 43,
	Motion_ChangeWepon2_SaberEarth = 44,

	//銃装備時の地上でダメージを受けたとき
	Motion_Damage_GunEarth = 45,

	//銃装備時の空中でダメージを受けたとき
	Motion_Damage_GunAir = 46,

	//剣装備時の地上でダメージを受けたとき
	Motion_Damage1_SaberEarth = 47,
	Motion_Damage2_SaberEarth = 48,

	//銃装備時に死んだ
	Motion_Die_GunEarth = 49,

	//銃装備時に空中で死んだ
	Motion_Die_GunAir = 50,

	//剣装備時に死んだ
	Motion_Die_SaberEarth = 51,
};

//ボスの高さ
const float BossHeight{ 1.f };

//衝突判定用の半径
const float BossRadius{ 0.6f };

//重力
const float Gravity_{ -0.016f };

//足元のオフセット
const float FootOffset{ 0.1f };

//振り返るときの速度
const float TurnAngle{ 2.5f };

//走るときの速さ
const float runSpeed{ 2.0f };

Boss::Boss(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Boss,Mesh_Boss ,Mesh_Boss,1,true },
	motion_{ Motion_Idle_GunEarth },
	state_{ State::Move },
	state_timer_{ 0.f }
{

	world_ = world;
	tag_ = "EnemyTag";
	name_ = "Boss";
	collider_ = BoundingSphere{ BossRadius,GSvector3{0.f,BossHeight,0.f} };
	transform_.position(position);
	mesh_.Transform(transform_.localToWorldMatrix());

	bossstate_ = new BossState();

	player_ = static_cast<Player*>(world_->find_actor("Player"));

}

Boss::~Boss() {
	delete bossstate_;
}

void Boss::update(float delta_time) {

	//移動速度
	walkSpeed = bossstate_->MoveSpeed();

	//状態の更新
	update_state(delta_time);

	//重力の更新
	velocity_.y += Gravity_ * delta_time;

	//重力を加える
	transform_.translate(0.f, velocity_.y, 0.f);

	//フィールドとの当たり判定
	collide_field();

	//モーションの変更
	mesh_.ChangeMotion(motion_, motion_loop_);

	//メッシュのモーションを更新
	mesh_.Update(delta_time);

	//ワールド変換行列を設定
	mesh_.Transform(transform_.localToWorldMatrix());

	if (gsGetKeyTrigger(GKEY_0)) {
		change_state(State::Move, Motion_Idle_GunEarth);
	}

}

void Boss::draw() const {

	mesh_.Draw();
	collider().draw();
}

void Boss::react(Actor& other) {

	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Die)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag") {

		//ダメージを受け取る関数
		damage_ = static_cast<BasicAttackCollider*>(&other)->GetAttackValue();

		//体力を減らす
		bossstate_->AddHP(-1);
		if (bossstate_->HP() <= 0) {
			//残りの体力がなければダウン状態に遷移
			change_state(State::Die, Motion_Die_GunEarth, false);
		}
		else {
			//弾の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//ダメージ状態に遷移する
			change_state(State::Damage, Motion_Damage_GunEarth, false);
		}
		return;
	}
	//プレイヤーまたは敵に衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}


}

BossState* Boss::bossState_() const
{
	return bossstate_;
}

void Boss::update_state(float delta_time) {

	switch (state_)
	{
	case Boss::State::Move:
		move(delta_time);
		break;
	case Boss::State::AttackMove:
		AttackMove(delta_time);
		break;
	case Boss::State::Shooting:
		attack(delta_time);
		break;
	case Boss::State::Slashing:
		attack(delta_time);
		break;
	case Boss::State::Damage:
		damage(delta_time);
		break;
	case Boss::State::Die:

		break;
	}

	state_timer_ += delta_time;

}

void Boss::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.f;
}

void Boss::move(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();

	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (TurnAngle * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -TurnAngle, TurnAngle) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);
	//前進する（ローカル座標）
	transform_.translate(0.f, 0.f, walkSpeed * delta_time);

	if (target_distance(player_->transform().position(), transform_.position()) <= 10) {
		change_state(Boss::State::AttackMove, Motion_MAttackF_GunEarth);
	}

}

void Boss::AttackMove(float delta_time) {

	if (target_distance(player_->transform().position(), transform_.position()) >= 10) {
		change_state(Boss::State::Move, Motion_WarkF_GunEarth);
	}

}

float Boss::target_signed_angle() {

	//プレイヤーの座標
	GSvector3 PlayerPos = player_->transform().position();

	//プレイヤーと自身の座標の方向ベクトル
	GSvector3 to_target = PlayerPos - transform_.position();

	GSvector3 forward = transform_.forward();

	to_target.y = 0;
	forward.y = 0;

	return GSvector3::signedAngle(forward, to_target);

}

float Boss::target_distance(GSvector3 Targetpos, GSvector3 pos) {
	return GSvector3::distance(Targetpos, pos);
}

void Boss::attack(float delta_time) {

}

void Boss::damage(float delta_time) {

	if (state_timer_ >= mesh_.MotionEndTime()) {

		change_state(Boss::State::Move, Motion_WarkF_GunEarth);
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
	line.end = position + GSvector3{ 0.f,-FootOffset,0.f };
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

