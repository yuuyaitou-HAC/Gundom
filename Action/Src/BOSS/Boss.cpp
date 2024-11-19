#include "Boss.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "Collision/BasicAttackCollider.h"
#include "EnemyBullet/BossAttackRange.h"

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
const float BossHeight_{ 1.f };

//衝突判定用の半径
const float BossRadius_{ 0.6f };

//重力
const float Gravity_{ -0.016f };

//足元のオフセット
const float FootOffset_{ 0.1f };

//振り返るときの速度
const float TurnAngle_{ 2.5f };

//走るときの速さ
const float RunSpeed_{ 2.0f };

//ある程度の誤差を強要するための閾値
const double EPSILON_ = 1e-9;

Boss::Boss(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Boss,Mesh_Boss ,Mesh_Boss,1,true },
	motion_{ Motion_Idle_GunEarth },
	state_{ State::Move },
	Atate_Timer_{ 0.f },
	WeaponDistance_{ 10.0f },
	IsFry_{ false }
{
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "Boss";
	collider_ = BoundingSphere{ BossRadius_,GSvector3{0.f,BossHeight_,0.f} };
	transform_.position(position);
	mesh_.Transform(transform_.localToWorldMatrix());

	//ボスステータスを生成
	bossstate_ = new BossState();

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//ボス弾管理クラスを生成
	GC_ = new BossGunController{ world_,transform_.position() };
}

Boss::~Boss() {

	//ボスで生成したものを削除
	delete GC_;
	delete bossstate_;

}

void Boss::update(float delta_time) {

	//移動速度
	WalkSpeed_ = bossstate_->MoveSpeed();

	//状態の更新
	update_state(delta_time);

	if (!IsFry_) {
		//重力の更新
		velocity_.y += Gravity_ * delta_time;
		//重力を加える
		transform_.translate(0.f, velocity_.y, 0.f);
	}
	if (IsFry_) {
		velocity_.y = 0;
	}
	//フィールドとの当たり判定
	collide_field();

	//モーションの変更
	mesh_.ChangeMotion(motion_, Motion_Loop_);

	//メッシュのモーションを更新
	mesh_.Update(delta_time);

	//ワールド変換行列を設定
	mesh_.Transform(transform_.localToWorldMatrix());

	MyPos_ = transform_.position();

	PlayerPos_ = player_->transform().position();

	//ボス弾管理クラスのアップデートを呼ぶ
	GC_->update(delta_time);

	//一定距離プレイヤーと近づいたら斬撃を放つ
	if (target_distance(PlayerPos_, MyPos_) <= 2) {
		change_state(Boss::State::Slashing, Motion_Attack1_SubarEath);
	}


	//デバック用　ボスをプレイヤーの場所にする
	if (gsGetKeyTrigger(GKEY_0)) {
		transform_.position(PlayerPos_);
	}

	//距離に応じて銃を切り替える
	changeGun();

	//飛ぶかどうかの判定
	changeFly();
}

void Boss::draw() const {

	mesh_.Draw();
	collider().draw();
	//ボス弾管理クラスの描画を呼ぶ
	GC_->draw();
}

void Boss::react(Actor& other) {

	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Die)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag") {

		//ダメージを受け取る関数
		Damage_ = static_cast<BasicAttackCollider*>(&other)->GetAttackValue();

		//体力を減らす
		bossstate_->AddHP(-Damage_);
		if (bossstate_->HP() <= 0) {
			//残りの体力がなければダウン状態に遷移
			change_state(State::Baster, Motion_Die_GunEarth, false);
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

//銃の切り替え
void Boss::changeGun() {

	//プレイヤーとの距離
	float distance = target_distance(PlayerPos_, MyPos_);

	//jyuugeki
	if (distance >= WeaponDistance_) {

		//銃の種類の変更(ビームライフル)してステータスを攻撃にする
		GC_->SetState(1);
		//ボスステータスの変更
		bossState_()->SetGunState(BossState::GunState::Beamlifl);

	}
	else {
		//銃の種類の変更(ガトリング)してステータスを攻撃にする
		GC_->SetState(2);
		//ボスステータスの変更
		bossState_()->SetGunState(BossState::GunState::Gatling);

	}

}

void Boss::update_state(float delta_time) {

	switch (state_)
	{
	case Boss::State::Move:
		move(delta_time);
		break;
	case Boss::State::AttackMove:
		attackMove(delta_time);
		break;
	case Boss::State::Slashing:
		slash(delta_time);
		break;
	case Boss::State::Damage:
		damage(delta_time);
		break;
	case Boss::State::Baster:
		baster(delta_time);
		break;
	case Boss::State::Die:
		death(delta_time);
		break;
	}

	Atate_Timer_ += delta_time;

}

void Boss::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	Motion_Loop_ = loop;
	state_ = state;
	Atate_Timer_ = 0.f;
}

//移動処理
void Boss::move(float delta_time) {

	//プレイヤーに方向を
	// 向かせる
	faceThePlayer(delta_time);

	//前進する（ローカル座標）
	transform_.translate(0.f, 0.f, WalkSpeed_ * delta_time);

	//プレイヤーと一定距離近づいたら
	if (target_distance(PlayerPos_, MyPos_) < 25) {

		//その場で攻撃開始
		change_state(Boss::State::AttackMove, Motion_Attack_GunEarth);

	}
}

//飛ぶかどうか
void Boss::changeFly() {

	//プレイヤーが自身より上にいる
	//プレイヤーの高さがジャンプの範疇を超えたとき
	if (PlayerPos_.y > 3) {
		IsFry_ = true;
	}
	else {
		IsFry_ = false;
	}
}

void Boss::attackMove(float delta_time) {

	//プレイヤーの方向を向かせる
	faceThePlayer(delta_time);

	MoveTimer_ -= delta_time;

	//一定時間で目標地点更新
	if (MoveTimer_ <= 0) {
		//移動ポイントの取得
		Attackpoint_ = attackPoint();

		MoveTimer_ = AsignmentMoveTimer_;
	}

	GSvector3 direction = Attackpoint_;

	direction.normalize();  // 方向を正規化する

	transform_.translate(direction.x * delta_time * WalkSpeed_, 0, direction.z * delta_time * WalkSpeed_);  // 正規化した方向に移動

	if (IsFry_) {
		fry(delta_time);
	}

	//弾を撃つ処理
	shoot(delta_time);

	//一定距離離れたら
	if (target_distance(PlayerPos_, MyPos_) >= 30) {
		change_state(Boss::State::Move, Motion_WarkF_GunEarth);
	}

}

//飛ぶ
//独自の方法
void Boss::fry(float delta_time) {


	FryTimer_ -= delta_time;

	//高さの設定
	if (FryTimer_ < 0) {
		Frypow_.y = gsRand(FryRand_.x, FryRand_.y) + PlayerPos_.y;

		FryTimer_ = AsignmentFryTimer_;
	}

	//目標地点に応じて速さの設定
	if (MyPos_.y > Frypow_.y) {
		transform_.translate(0, -WalkSpeed_, 0);
	}
	else {
		transform_.translate(0, WalkSpeed_, 0);
	}

	GSvector3 myposy = MyPos_;
	myposy.x = myposy.z = 0;

	//目標の高さ到達後にその場にとどめる
	if (target_distance(myposy, Frypow_) < 1) {
		Frypow_.y = 0;
	}

}

GSvector3 Boss::attackPoint() {

	Point_ = GSvector3{ (float)gsRand(-10,10),0,(float)gsRand(-10,10) };

	Point_ += PlayerPos_;

	float distance = target_distance(PlayerPos_, Point_);


	if (distance >= 5 && distance <= 10 && !onTheLine(Point_)) {
		return Point_;
	}

	return attackPoint();

}

//目標地点がざひょうじょうにいるかどうか
bool Boss::onTheLine(GSvector3 point)
{
	double x1 = PlayerPos_.x;
	double y1 = PlayerPos_.y;
	double z1 = PlayerPos_.z;
	double x2 = MyPos_.x;
	double y2 = MyPos_.y;
	double z2 = MyPos_.z;
	double xc = point.x;
	double yc = point.y;
	double zc = point.z;

	// 同一直線上にあるか (比が一致するかを確認)
	double t1 = (x2 - x1) != 0 ? (xc - x1) / (x2 - x1) : 0;
	double t2 = (y2 - y1) != 0 ? (yc - y1) / (y2 - y1) : 0;
	double t3 = (z2 - z1) != 0 ? (zc - z1) / (z2 - z1) : 0;

	// 比がほぼ同じかを確認
	if (fabs(t1 - t2) > EPSILON_ || fabs(t2 - t3) > EPSILON_ || fabs(t1 - t3) > EPSILON_) {
		return false;
	}

	// 線分上にあるか
	if (MIN(x1, x2) <= xc && xc <= MAX(x1, x2) &&
		MIN(y1, y2) <= yc && yc <= MAX(y1, y2) &&
		MIN(z1, z2) <= zc && zc <= MAX(z1, z2)) {
		return true;
	}

	return false;
}

void Boss::shoot(float delta_time) {

	ShootTime_ += delta_time;
	//銃の種類がビームライフルなら
	if (bossState_()->gunstate_() == BossState::GunState::Beamlifl) {
		//残弾があり一定時間たったら
		if (bossState_()->BeamBullet() > 0 && ShootTime_ >= 20) {
			GC_->Fire();

			ShootTime_ = 0;

		}

	}
	//銃の種類がガトリングなら
	else if (bossState_()->gunstate_() == BossState::GunState::Gatling) {

		if (bossState_()->GatlingBullet() > 0 && ShootTime_ >= 5) {
			GC_->Fire();

			ShootTime_ = 0;
		}
	}

}

//バスターライフル発射
void Boss::baster(float delta_time) {

	BasterTimer_ -= delta_time;


	if (BasterTimer_ <= 0) {
		//銃の種類の変更(ビームライフル)してステータスを攻撃にする
		GC_->SetState(3);
		//ボスステータスの変更
		bossState_()->SetGunState(BossState::GunState::Basterlifl);

		GC_->Fire();

		change_state(State::Die, Motion_Die_GunEarth);
	}

}

//斬撃
void Boss::slash(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);


	GSvector3 pos = MyPos_ + transform_.forward() * SlashDistance_;
	pos.y += SlashHight_;

	//斬撃の生成
	world_->add_actor(new BossAttackRange{ world_,pos,GSvector3().zero(),10 });

	retreat();

}

//自身の後ろに後退
void Boss::retreat() {

	Rotate_ = transform_.forward();

	Rotate_.y += 0.2f;

	GSvector3 bossrotate = Rotate_.normalize();

	GSvector3 br = bossrotate * 0.4f;

	for (int i = 0; i < 10; i++) {
		transform_.translate(-br, GStransform::Space::World);

	}

	change_state(Boss::State::Move, Motion_WarkF_GunEarth);
}


void Boss::damage(float delta_time) {

	//ダメージモーションが終了したら移動ステータスにする
	if (Atate_Timer_ >= mesh_.MotionEndTime()) {

		change_state(Boss::State::Move, Motion_WarkF_GunEarth);
	}

}

void Boss::death(float delta_time) {

	//モーションが終わったら死亡
	if (Atate_Timer_ >= mesh_.MotionEndTime()) {
		die();
	}

}

void Boss::faceThePlayer(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();

	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (TurnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -TurnAngle_, TurnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

}

//慣性
void Boss::inertia(float delta_time) {

}

//プレイヤーと自身のなす角度
float Boss::target_signed_angle() {

	//プレイヤーと自身の座標の方向ベクトル
	GSvector3 to_target = PlayerPos_ - MyPos_;

	GSvector3 forward = transform_.forward();

	to_target.y = 0;
	forward.y = 0;

	return GSvector3::signedAngle(forward, to_target);

}

//2点間の距離
float Boss::target_distance(GSvector3 Targetpos, GSvector3 pos) {
	return GSvector3::distance(Targetpos, pos);
}

void Boss::collide_actor(Actor& other) {

	//y座標を除く座標を求める
	GSvector3 position = MyPos_;
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
	line.end = position + GSvector3{ 0.f,-FootOffset_,0.f };
	GSvector3 intersect;//地面との交点
	if (world_->field()->collide(line, &intersect)) {
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.f;
		IsFry_ = false;
	}
}

