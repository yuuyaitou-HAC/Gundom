#include "HBM.h"
#include "Collision/AttackCollider.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "Collision/BasicAttackCollider.h"
#include "EnemyBullet/BossBeamRifleBullet.h"
#include "EnemyBullet/GatlingBullet.h"
#include "EnemyBullet/EnemyAttackRange.h"
#include "EnemyBullet/SniperBullet.h"

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

//高さと幅
const float Height{ 1.f };
const float Radius{ 0.5f };

//重力
const float Gravity_{ -0.016 };

//弾の発射のための高さ調整
const float FootOffset{ 0.1f };

//振り向き速度
const float TurnAngle{ 2.5f };

//移動速度
const float WalkSpeed{ 0.1f };

const float RunSpeed{ 0.4f };

//コンストラクタ
HBM::HBM(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_HBM,Mesh_HBM,Mesh_HBM,Motion_Idle_GunEarth,true },
	motion_{ Motion_Idle_GunEarth },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.f },
	player_{ nullptr },
	health_{ 2 } {

	world_ = world;

	tag_ = "EnemyTag";
	name_ = "HBM";

	collider_ = BoundingSphere{ Radius,GSvector3{0.f,Height,0.f} };

	transform_.position(position);

	mesh_.Transform(transform_.localToWorldMatrix());

	damage_ = 0;

	//プレイヤーを取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//攻撃の間隔を代入
	AttackTimer = gsRand(RandSlashTime.x, RandSlashTime.y);

	AttackMoveTimer = 0.0f;
}

//更新
void HBM::update(float delta_time) {

	update_state(delta_time);

	//重力
	velocity_.y += Gravity_ * delta_time;
	transform_.translate(0.f, velocity_.y, 0.f);

	collide_field();

	mesh_.ChangeMotion(motion_, motion_loop_);

	mesh_.Update(delta_time);

	mesh_.Transform(transform_.localToWorldMatrix());

	//自身の座標
	pos = transform_.position();
}

//描画
void HBM::draw() const {
	if (state_ != State::Die)mesh_.Draw();

	collider().draw();
}

//武器描画
void HBM::drawWeapon() {

}

//当たり判定
void HBM::react(Actor& other) {
	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Die)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag") {

		//ダメージを受け取る関数
		damage_ = static_cast<BasicAttackCollider*>(&other)->GetAttackValue();

		//体力を減らす
		health_--;
		if (health_ <= 0) {
			tag_ = "DieEnemyTag";

			//武器ごとのプレイヤーのスキルポイント量を変える
			switch (weapon)
			{
			case 1:
				player_->playerState_()->setExSkillPoint(50);
				break;
			case 2:
				player_->playerState_()->setExSkillPoint(50);
				break;
			case 3:
				player_->playerState_()->setExSkillPoint(50);
				break;
			case 4:
				player_->playerState_()->setExSkillPoint(50);
				break;
			}

			//斬撃
			if (weapon == 1) {
				change_state(State::Die, Motion_Die_SaberEarth, false);
			}
			//銃
			else {
				change_state(State::Die, Motion_Die_GunEarth, false);
			}
		}

		else {
			//弾の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;

			//ダメージ状態に遷移する
			//斬撃
			if (weapon == 1) {
				change_state(State::Damage, Motion_Die_SaberEarth, false);
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
		change_state(State::Move, 0);
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

	Destination = pos;

}

//攻撃手段
void HBM::AttackingStrategy(int num) {

	weapon = num;

}

//AI側に攻撃中かどうかを知らせる
bool HBM::AttakFlag() {

	return SlashAttackFlag;
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

	//状態タイマーの更新
	state_timer_ += delta_time;

}


void HBM::change_state(State state, GSuint motion, bool loop) {
	//モーション番号の更新
	motion_ = motion;
	//モーションのループ指定
	motion_loop_ = loop;
	//状態の更新
	state_ = state;
	//状態タイマの初期化
	state_timer_ = 0.f;
}

//アイドル
void HBM::idle(float delta_time) {
	//何もなければ、アイドル状態のまま
	if (weapon == 1) {
		change_state(State::Idle, Motion_Idle_SaberEarth);
	}
	else {

		change_state(State::Idle, Motion_Idle_GunEarth);
	}
}

//移動
void HBM::move(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (TurnAngle * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -TurnAngle, TurnAngle) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);
	//移動
	GSvector3 moveto = Destination - transform_.position();
	transform_.translate(moveto.normalized() * RunSpeed * delta_time, GStransform::Space::World);

	//目標地点に到達したら攻撃開始
	if (target_distance() <= 1.5f) {

		change_state(State::Attack, 0);
	}
}

//攻撃
void HBM::attack(float delta_time) {

	//プレイヤーの方向を向かせる
	faceThePlayer(delta_time);

	//武器によって攻撃時の処理を変える
	switch (weapon)
	{
	case 1:
		SlashingMove(delta_time);
		break;
	case 2:
		Gatring(delta_time);
		break;
	case 3:
		BeamLifre(delta_time);
		break;
	case 4:
		Snaiper(delta_time);
		break;
	}
}

//ビームサーベル装備中の移動
void HBM::SlashingMove(float delta_time) {

	//攻撃までの時間
	AttackTimer -= delta_time;

	//次の移動までの時間
	AttackMoveTimer -= delta_time;

	if (AttackMoveTimer <= 0) {
		sign_ = sign();

		//ランダムな時間を入れる
		AttackMoveTimer = gsRand(AttackRandSabel.x, AttackRandSabel.y);
	}

	//移動
	transform_.translate(transform_.position().right() * sign_ * WalkSpeed * delta_time);

	if (AttackTimer <= 0) {

		SlashAttackFlag = true;

		//前進
		transform_.translate(0.f, 0.f, RunSpeed * delta_time);

		float playerDistance = GSvector3::distance(transform_.position(), player_->transform().position());


		if (playerDistance <= 5) {

			switch (gsRand(1, 1))
			{
			case 1:
				change_state(State::Slashing, Motion_Attack_GunEarth);
				break;
			case 2:
				change_state(State::FeintSlashing, Motion_Attack_GunEarth);
				break;
			}
		}
	}
}

//ビームサーベルで攻撃
void HBM::SlashingAttack(float delta_time) {

	float playerDistance = GSvector3::distance(transform_.position(), player_->transform().position());

	if (playerDistance <= 1 && !SlasingAttackFrag) {
		generate_bullet();

		SlasingAttackFrag = true;
	}

	if (SlasingAttackFrag) {
		transform_.translate(0.f, 0.f, -RunSpeed * delta_time);

		if (playerDistance > 10) {
			AttackTimer = gsRand(RandSlashTime.x, RandSlashTime.y);
			SlashAttackFlag = false;
			change_state(State::Attack, Motion_Attack_GunEarth);
			SlasingAttackFrag = false;
		}
	}
	else {
		transform_.translate(0.f, 0.f, RunSpeed * delta_time);
	}
}

//ビームサーベル装備時のフェイント
void HBM::SlashingFeint(float delta_time) {

	transform_.translate(0.f, 0.f, -RunSpeed * delta_time);

	float a = GSvector3::distance(transform_.position(), player_->transform().position());

	if (a > 10) {
		AttackTimer = gsRand(RandSlashTime.x, RandSlashTime.y);
		SlashAttackFlag = false;

		change_state(State::Attack, Motion_Attack_GunEarth);
	}

}

//ガトリングで攻撃
void HBM::Gatring(float delta_time) {

	//攻撃時間
	AttackTimer -= delta_time;

	//移動地点更新時間
	AttackMoveTimer -= delta_time;

	float a = GSvector3::distance(pos, Destination);

	if (a >= 5) {
		transform_.translate((Destination - pos).normalized() * WalkSpeed / 2, GStransform::Space::World);
	}
	else {

		if (AttackMoveTimer <= 0) {
			sign_ = sign();
			AttackMoveTimer = gsRand(AttackRandGatling.x, AttackRandGatling.y);
		}
		transform_.translate(transform_.localEulerAngles().right() * sign_ * WalkSpeed / 2);
	}


	if (AttackTimer <= 0) {
		generate_bullet();
		AttackTimer = 20.0f;
	}
}

//ビームライフルで攻撃
void HBM::BeamLifre(float delta_time) {

	//攻撃時間
	AttackTimer -= delta_time;

	//移動地点更新時間
	AttackMoveTimer -= delta_time;

	if (AttackMoveTimer <= 0) {

		sign_ = sign();

		AttackMoveTimer = gsRand(AttackRandBeamRifle.x, AttackRandBeamRifle.y);
	}

	transform_.translate(transform_.localPosition().right() * sign_ * WalkSpeed);

	if (AttackTimer <= 0) {

		generate_bullet();

		AttackTimer = 60.0f;

	}
}

//スナイパーで攻撃
void HBM::Snaiper(float delta_time) {

	//攻撃時間
	AttackTimer -= delta_time;

	if (AttackTimer <= 0) {

		generate_bullet();

		AttackTimer = 60.0f;
	}
}

//ダメージ
void HBM::damage(float delta_time) {

	//ノックバックする
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	velocity_ -= GSvector3{ velocity_.x,0.f,velocity_.z }*0.5f * delta_time;

	change_state(State::Move, 0);
}

//退却
void HBM::runaway(float delta_time) {

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
	transform_.translate(0.f, 0.f, RunSpeed * delta_time);

	//目標地点に到達したら死亡状態にする
	if (target_distance() <= 1.5f) {

		tag_ = "DieEnemyTag";

		change_state(State::Die, 0);
	}
}

//死
void HBM::Die(float delta_time) {
	if (DieProcessing == 0)DieProcessing++;
	//爆発エフェクトの再生
}

//弾生成
void HBM::generate_bullet() {

	GSvector3 position = pos + transform_.forward();
	GSvector3 velocity;

	if (weapon == 2) {
		//ガトリングの弾を拡散させる
		velocity = ((player_->transform().position() - position) + GSvector3{ gsRandf(-3,3), gsRandf(-3,3), gsRandf(-3,3) }).normalized() * 0.5f;
	}
	else {
		velocity = (player_->transform().position() - position).normalized() * 0.5f;
	}

	position.y += 1.5f;

	switch (weapon)
	{
	case 1:
		world_->add_actor(new EnemyAttackRange{ world_,position,GSvector3().zero(),10 });
		break;
	case 2:
		world_->add_actor(new GatlingBullet{ world_,position,velocity,5 });
		break;
	case 3:
		world_->add_actor(new BossBeamRifleBullet{ world_,position,velocity,10 });
		break;
	case 4:
		world_->add_actor(new SniperBullet{ world_,position,velocity * 2 ,20 });
		break;
	}
}

//移動時に呼ばれるもの　目標地点との差を出す
float HBM::target_signed_angle() {

	//自身とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_target = Destination - transform_.position();
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
	GSvector3 to_target = player_->transform().position() - transform_.position();
	//自身の前ベクトルを求める
	GSvector3 forward = transform_.forward();

	//ベクトルのy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;

	return GSvector3::signedAngle(forward, to_target);
}

//自身と目標との間
float HBM::target_distance() {
	return GSvector3::distance(Destination, transform_.position());
}

//プレイヤーの方向を向かせる
void HBM::faceThePlayer(float delta_time) {

	float angle;

	//ステータスに応じて向く方向を変える
	if (HBM::state_ == State::Attack)angle = target_signed_angle_fire();
	else angle = target_signed_angle();

	if (std::abs(angle) > (TurnAngle * delta_time))angle = CLAMP(angle, -TurnAngle, TurnAngle) * delta_time;

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
	line.end = position + GSvector3{ 0.f,-FootOffset,0.f };
	GSvector3 intersect;//地面との交差
	if (world_->field()->collide(line, &intersect)) {
		//交差した点からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.f;
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
