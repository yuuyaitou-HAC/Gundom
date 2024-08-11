#include "Enemy/Enemy.h"
#include "Collision/AttackCollider.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"

//敵のモーション番号
enum {
	MotionIdle      = 0, //アイドル
	MotionWalk      = 1, //歩き
	MotionTurnLeft  = 2, //左に振り向く
	MotionTurnRight = 3, //右に振り向く
	MotionDamage    = 4, //ダメージ
	MotionAttack    = 5, //攻撃
	MotionDown      = 6, //ダウン
};

//振り向き判定の距離
const float TurnDistance{ 1.5f };
//攻撃判定の距離
const float AttackDistance{ 1.2f };
//移動判定の距離
const float WalkDistance{ 10.0f };
//移動スピード
const float WalkSpeed{ 0.025f };
//振り向く角度
const float TurnAngle{ 2.5f };

//自分の速さ
const float EnemyHeight{ 1.f };
//衝突判定用の半径
const float EnemyRadius{ 0.6f };
//足元のオフセット
const float FootOffset{ 0.1f };
//重力
const float Gravity{ -0.016f };

//コンストラクタ
Enemy::Enemy(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Enemy,Mesh_Enemy,Mesh_Enemy,MotionIdle,true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.f },
	player_{ nullptr },
	health_{ 5 }{
	//ワールド設定
	world_ = world;
	//タグ名の設定
	tag_ = "EnemyTag";
	//名前の設定
	name_ = "Enemy";
	//衝突判定球の設定
	collider_ = BoundingSphere{ EnemyRadius,GSvector3{0.f,EnemyHeight,0.f} };
	//座標の初期化
	transform_.position(position);
	//ワールド返還行列の初期化
	mesh_.Transform(transform_.localToWorldMatrix());
}

//更新
void Enemy::update(float delta_time) {
	//プレイヤーを検索する
	player_ = world_->find_actor("Player");
	//状態の更新
	update_state(delta_time);
	//重力を更新
	velocity_.y += Gravity * delta_time;
	//重力を加える
	transform_.translate(0.f, velocity_.y, 0.f);
	//フィールドとの衝突判定
	collide_field();
	//モーション変更
	mesh_.ChangeMotion(motion_, motion_loop_);
	//メッシュを更新
	mesh_.Update(delta_time);
	//行列を設定
	mesh_.Transform(transform_.localToWorldMatrix());
}

//描画
void Enemy::draw()const {
	//メッシュの描画
	mesh_.Draw();
	//衝突判定用のデバック表示
	collider().draw();
}

//衝突処理
void Enemy::react(Actor& other) {
	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Down)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag") {
		//体力を減らす
		health_--;
		if (health_ <= 0) {
			//残りの体力がなければダウン状態に遷移
			change_state(State::Down, MotionDown, false);
		}
		else {
			//弾の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//ダメージ状態に遷移する
			change_state(State::Damage, MotionDamage, false);
		}
		return;
	}
	//プレイヤーまたは敵に衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

//状態の更新
void Enemy::update_state(float delta_time) {
	//各状態に分岐する
	switch (state_) {
	//case State::Idle:   idle(delta_time);   break;
	case State::Attack: attack(delta_time); break;
	case State::Walk:   walk(delta_time);   break;
	case State::Damage: damage(delta_time); break;
	case State::Turn:   turn(delta_time);   break;
	case State::Down:   down(delta_time);   break;
	}
	//状態タイマの更新
	state_timer_ += delta_time;
}

//状態の変更
void Enemy::change_state(State state, GSuint motion, bool loop) {
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
void Enemy::idle(float delta_time) {
	//攻撃するか？
	if (is_attack()) {
		//攻撃判定
		generate_attack_collider();
		//攻撃状態に遷移
		change_state(State::Attack, MotionAttack);
		return;
	}
	//歩くか？
	if (is_walk()) {
		//移動状態に遷移
		change_state(State::Walk, MotionWalk);
		return;
	}
	//振り向くか？
	if (is_turn()) {
		//左に振り向くか？右に振り向くか？
		GSuint motion = (target_signed_angle() >= 0.f) ? MotionTurnLeft : MotionTurnRight;
		//振り向き状態に遷移
		change_state(State::Turn, motion);
		return;
	}
	//何もなければ、アイドル状態のまま
	change_state(State::Idle, MotionIdle);
}

//移動中
void Enemy::walk(float delta_time) {
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
	transform_.translate(0.f, 0.f, WalkSpeed * delta_time);
	//攻撃するか？
	if (is_attack()) {
		//攻撃判定を生成
		generate_attack_collider();
		//攻撃状態に遷移する
		change_state(State::Attack, MotionAttack);
	}
}

//振り向き中
void Enemy::turn(float delta_time) {
	if (state_timer_ >= mesh_.MotionEndTime()) {
		//振り向きモーションが終了したらアイドル状態に遷移
		idle(delta_time);
	}
	else {
		//振り向きモーションしながらターゲットの方向を向く
		float angle = (target_signed_angle() >= 0.f) ? TurnAngle : -TurnAngle;
		transform_.rotate(0.f, angle * delta_time, 0.f);
	}
}

//攻撃中
void Enemy::attack(float delta_time) {
	if (state_timer_ >= mesh_.MotionEndTime()) {
		//攻撃モーションが終了したらアイドル状態に遷移
		idle(delta_time);
	}
}

//ダメージ中
void Enemy::damage(float delta_time) {
	if (state_timer_ < mesh_.MotionEndTime()) {
		//ノックバックする
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
		velocity_ -= GSvector3{ velocity_.x,0.f,velocity_.z }*0.5f * delta_time;
		return;
	}
	//ダメージモーション終了後、ターゲット方向との角度差が90.0度以上（背後）なら振り向き中に遷移
	if (target_angle() >= 90.f) {
		//振り向き中に遷移
		GSuint motion = (target_signed_angle() >= 0.f) ? MotionTurnLeft : MotionTurnRight;
		change_state(State::Turn, motion);
	}
	else {
		//アイドル状態に遷移
		idle(delta_time);
	}
}

//ダウン中
void Enemy::down(float delta_time) {
	if (state_timer_ >= mesh_.MotionEndTime()) {
		//ダウンモーションが終了したら死亡
		die();
	}
}

//攻撃判定を生成する
void Enemy::generate_attack_collider() {
	//
	const float AttackColliderDistance{ 0.5f };
	//
	const float AttackColliderRadius{ 0.3f };
	//
	const float AttackColliderHeight{ 1.f };

	//
	const float AttackColliderDelay{ 15.f };
	//
	const float AttackColliderSpan{ 5.f };

	//
	GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
	//
	position.y += AttackColliderHeight;
	//
	BoundingSphere collider{ AttackColliderRadius,position };
	//
	world_->add_actor(new AttackCollider{ world_,collider,
		"EnemyAttackTag","EnemyAttack",tag_,AttackColliderSpan,AttackColliderDelay });
}

//振り向き判定
bool Enemy::is_turn()const {
	//振り向き距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が90.0度以上か？
	return (target_distance() <= TurnDistance) && (target_angle() >= 90.f);
}

//攻撃判定
bool Enemy::is_attack()const {
	//攻撃範囲内かつ前方向と前向き方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？
	return (target_distance() <= AttackDistance) && (target_angle() <= 20.f);
}

//移動判定
bool Enemy::is_walk()const {
	//移動距離内かつ前方向と前向き方向のベクトルとターゲット方向のベクトルの角度差が100.0度以下か？
	return (target_distance() <= WalkDistance) && (target_angle() <= 100.f);
}

//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号付）
float Enemy::target_signed_angle()const {
	//ターゲットがいなければ0を返す
	if (player_ == nullptr)return 0.f;
	//ターゲット方向のベクトルを求める
	GSvector3 to_target = player_->transform().position() - transform_.position();
	//前向き方向のベクトルを取得
	GSvector3 forward = transform_.forward();
	//ベクトルのy成分を無効にする
	forward.y = 0.f;
	to_target.y = 0.f;
	//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
	return GSvector3::signedAngle(forward, to_target);
}

//前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号なし）
float Enemy::target_angle()const {
	return std::abs(target_signed_angle());
}

//ターゲットとの距離を求める
float Enemy::target_distance()const {
	//ターゲットがいなければ最大距離を返す
	if (player_ == nullptr)return FLT_MAX;
	//ターゲットとの距離を計算する
	return GSvector3::distance(player_->transform().position(), transform_.position());
}

//フィールドとの衝突判定
void Enemy::collide_field() {
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

//アクターとの衝突処理
void Enemy::collide_actor(Actor& other) {
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