#include "Tank.h"
#include "Collision/AttackCollider.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Collision/BasicAttackCollider.h"
#include "EnemyBullet/TankBullet.h"
#include "Player/Player.h"

//アニメーション
enum {
	MotionIdle = 0, //アイドル
	MotionNull = -1,
};

//自分の高さ
const float EnemyHeight{ 1.f };
//衝突判定用の半径
const float EnemyRadius{ 0.6f };

//重力
const float Gravity_{ -0.016f };

const float FootOffset{ 0.1f };

//振り向く角度
const float TurnAngle{ 2.5f };

//移動速度
const float WalkSpeed{ 0.2f };

//コンストラクタ
Tank::Tank(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Enemy,Mesh_Enemy,Mesh_Enemy,MotionIdle,true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.f },
	player_{ nullptr },
	health_{ 2 } {

	//ワールド設定
	world_ = world;

	//タグ名の設定
	tag_ = "EnemyTag";

	//名前の設定
	name_ = "Tank";

	//衝突判定球の設定
	collider_ = BoundingSphere{ EnemyRadius,GSvector3{0.f,EnemyHeight,0.f} };

	//座標の初期化
	transform_.position(position);

	//ワールド返還行列の初期化
	mesh_.Transform(transform_.localToWorldMatrix());

	//受けたダメージ初期化
	damage_ = 0;

	//プレイヤー取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));
}

//更新
void Tank::update(float delta_time) {

	//状態の更新
	update_state(delta_time);

	//重力の更新
	velocity_.y += Gravity_ * delta_time;

	//重力を加える
	transform_.translate(0.f, velocity_.y, 0.f);

	//フィールドとの当たり判定
	collide_field();

	//モーション更新
	mesh_.ChangeMotion(motion_, motion_loop_);

	//メッシュを更新
	mesh_.Update(delta_time);

	//行列を設定	
	mesh_.Transform(transform_.localToWorldMatrix());
}

//描画
void Tank::draw() const {

	if (state_ != State::Die)mesh_.Draw();
	//衝突判定用のデバック表示
	collider().draw();
}

//衝突判定
void Tank::react(Actor& other) {

	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Die)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag") {

		//ダメージを受け取る関数
		damage_ = static_cast<BasicAttackCollider*>(&other)->GetAttackValue();

		//体力を減らす
		health_--;
		if (health_ <= 0) {

			//KILL数をカウント
			if (other.name() == "BeamSaberBullet") {
				world_->gameData()->setBeamSaberKillCounter(1);
			}
			else if (other.name() == "BeamRifleBullet") {
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

			tag_ = "DieEnemyTag";

			//EXスキルポイント加算
			player_->playerState_()->setExSkillPoint(5);

			//残りの体力がなければダウン状態に遷移
			change_state(State::Die, MotionNull, false);
		}
		else {
			//弾の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//ダメージ状態に遷移する
			change_state(State::Damage, MotionNull, false);
		}
		return;
	}
	//プレイヤーまたは敵に衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

//Ai側からのステータス変更
void Tank::ChangeState(int state) {

	switch (state) {

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
		change_state(State::Damage, 0);
		break;
	case 5:
		change_state(State::RunAway, 0);
		break;
	case 6:
		change_state(State::Die, 0);
		break;
	}
}

//Ai側に現在のステータスを返す
int Tank::StateNow() {

	switch (state_)
	{
	case Tank::State::Idle:
		return 1;
		break;
	case Tank::State::Move:
		return 2;
		break;
	case Tank::State::Attack:
		return 3;
		break;
	case Tank::State::Damage:
		return 4;
		break;
	case Tank::State::RunAway:
		return 5;
		break;
	case Tank::State::Die:
		return 6;
		break;
	}
}

void Tank::AttackPoint(GSvector3 pos) {
	Destination = pos;
}

//ステータス更新
void Tank::update_state(float delta_time) {

	//各状態に分岐する
	switch (state_)
	{
	case Tank::State::Idle:
		idle(delta_time);
		break;
	case Tank::State::Move:
		move(delta_time);
		break;
	case Tank::State::Attack:
		attack(delta_time);
		break;
	case Tank::State::Damage:
		damage(delta_time);
		break;
	case Tank::State::RunAway:
		runaway(delta_time);
		break;
	case Tank::State::Die:
		Die(delta_time);
		break;
	}
	//状態タイマーの更新
	state_timer_ += delta_time;
}

//ステータス変化
void Tank::change_state(State state, GSuint motion, bool loop) {

	//モーション番号の更新
	motion_ = motion;
	//モーションのループ指定
	motion_loop_ = loop;
	//状態の更新
	state_ = state;
	//状態タイマの初期化
	state_timer_ = 0.f;
}

//アイドル状態
void Tank::idle(float delta_time) {
	//何もなければ、アイドル状態のまま
	change_state(State::Idle, MotionIdle);
}

//移動
void Tank::move(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (TurnAngle * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -TurnAngle, TurnAngle) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

	GSvector3 moveto = Destination - transform_.position();
	transform_.translate(moveto.normalized() * WalkSpeed * delta_time, GStransform::Space::World);

	//目標地点に到達したら攻撃開始
	if (target_distance() <= 1.5f) 	change_state(State::Attack, 0);
}

//攻撃
void Tank::attack(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle_fire();
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (TurnAngle * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -TurnAngle, TurnAngle) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

	attacktime -= delta_time;

	//攻撃
	if (attacktime <= 0) {
		generate_bullet();
		attacktime = gsRand(randattacktime.x, randattacktime.y);
	}
}

//ダメージ
void Tank::damage(float delta_time) {

	//ノックバックする
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	velocity_ -= GSvector3{ velocity_.x,0.f,velocity_.z }*0.5f * delta_time;

	change_state(State::Move, 0);
}

//退却
void Tank::runaway(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (TurnAngle * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -TurnAngle, TurnAngle) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

	GSvector3 moveto = Destination - transform_.position();
	transform_.translate(moveto.normalized() * WalkSpeed * delta_time, GStransform::Space::World);

	//目標地点に到達したら死亡状態にする
	if (target_distance() <= 1.5f) {
		tag_ = "DieEnemyTag";
		change_state(State::Die, 0);
	}
}

//死亡
void Tank::Die(float delta_time) {

	//爆発エフェクトの再生
}

void Tank::generate_bullet() {

	GSvector3 pos = transform_.position();

	pos.y += 1.0f;

	GSvector3 playerpos = player_->transform().position();

	playerpos.y += 1.0f;

	GSvector3 velocity = (playerpos - pos).normalized();

	world_->add_actor(new TankBullet{ world_,pos,velocity ,5 });
}

void Tank::collide_field() {

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

void Tank::collide_actor(Actor& other) {

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

//移動時に呼ばれるもの　目標地点との差を出す
float Tank::target_signed_angle()
{
	//自身とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_target = Destination - transform_.position();
	//自身の前ベクトルを求める
	GSvector3 forward = transform_.forward();

	//ベクトルのy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;

	return GSvector3::signedAngle(forward, to_target);
}

//射撃時に呼ばれるもの　プレイヤーとの間を出す
float Tank::target_signed_angle_fire()
{
	if (player_ == nullptr)return 0.0f;

	//プレイヤーの座標取得
	Destination = player_->transform().position();

	//自身とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_target = Destination - transform_.position();
	//自身の前ベクトルを求める
	GSvector3 forward = transform_.forward();

	//ベクトルのy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;

	return GSvector3::signedAngle(forward, to_target);
}

float Tank::target_distance() {
	return GSvector3::distance(Destination, transform_.position());
}