#include "Tank.h"
#include "Collision/AttackCollider.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Collision/BasicAttackCollider.h"

//アニメーション
enum {

	MotionIdle = 0, //アイドル
	MotionWalk = 1, //歩き
	MotionTurnLeft = 2, //左に振り向く
	MotionTurnRight = 3, //右に振り向く
	MotionDamage = 4, //ダメージ
	MotionAttack = 5, //攻撃
	MotionDown = 6, //ダウン

};

//自分の速さ
const float EnemyHeight{ 1.f };
//衝突判定用の半径
const float EnemyRadius{ 0.6f };

//重力
const float Gravity_{ -0.016f };

const float FootOffset{ 0.1f };

//コンストラクタ
Tank::Tank(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Enemy,Mesh_Enemy,Mesh_Enemy,MotionIdle,true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Idle },
	state_timer_{ 0.f },
	player_{ nullptr },
	health_{ 200 } {

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

}

//更新
void Tank::update(float delta_time) {

	//プレイヤーを検索
	player_ = world_->find_actor("Player");

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

	//メッシュの描画
	mesh_.Draw();
	//衝突判定用のデバック表示
	collider().draw();


	if (damage_ >= 1) {
		gsTextPos(100, 100);
		gsDrawText("受けたダメージ = %d", damage_);

	}

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
		//health_--;
		if (health_ <= 0) {
			//残りの体力がなければダウン状態に遷移
			change_state(State::Die, MotionDown, false);
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
		die(delta_time);
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

}

//移動
void Tank::move(float delta_time) {

}

//攻撃
void Tank::attack(float delta_time) {

}

//ダメージ
void Tank::damage(float delta_time) {


}

//退却
void Tank::runaway(float delta_time) {

}

//死亡
void Tank::die(float delta_time) {

}

void Tank::collide_field(){

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

void Tank::collide_actor(Actor& other){

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
