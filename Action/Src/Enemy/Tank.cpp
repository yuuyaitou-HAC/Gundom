#include "Tank.h"
#include "Collision/AttackCollider.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Collision/BasicAttackCollider.h"
#include "EnemyBullet/TankBullet.h"
#include "Player/Player.h"
#include "GSeffect.h"

//コンストラクタ
Tank::Tank(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Enemy,Mesh_Enemy,Mesh_Enemy,NULL,true },
	state_{ State::Idle },
	state_timer_{ 0.f },
	health_{ 100 },
	defensive_{ 10 },
	attackValue_{ 30 },
	drawMeshFrag_{ true },
	playExplosionEffect_{ false } {

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

	mypos_ = transform_.position();

	int distance = GSvector3::distance(mypos_, player_->transform().position());
	//距離に応じてエフェクト再生するかどうかのフラグを変える
	if (distance >= 30)playEffectDistance_ = false;
	else playEffectDistance_ = true;

	//状態の更新
	update_state(delta_time);

	//重力の更新
	velocity_.y += gravity_ * delta_time;

	//重力を加える
	transform_.translate(0.f, velocity_.y, 0.f);

	//フィールドとの当たり判定
	collide_field();

	//メッシュを更新
	mesh_.Update(delta_time);

	//行列を設定	
	mesh_.Transform(transform_.localToWorldMatrix());
}

//描画
void Tank::draw() const {
	if (drawMeshFrag_)mesh_.Draw();
}

//衝突判定
void Tank::react(Actor& other) {

	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Die)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag") {

		if (playEffectDistance_) {
			//ヒットエフェクトの再生
			effectHit_ = gsPlayEffect(Effect_Hit, &mypos_);
			gsPlaySE(SE_Damage1);
		}

		//ダメージを受け取る関数
		damage_ = static_cast<BasicAttackCollider*>(&other)->GetAttackValue() - defensive_;

		//自身の防御力が上回った場合受けるダメージ量を０にする
		if (damage_ <= 0) {
			damage_ = 0;
		}

		//体力を減らす
		health_ -= damage_;
		if (health_ <= 0) {

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

			//ファンエルによる攻撃以外で死亡した場合プレイヤーのEXスキルポイントに加算
			if (other.name() != "AllRangeBullet")player_->player_state()->addExSkillPoint(30);

			//残りの体力がなければダウン状態に遷移
			change_state(State::Die);
		}
		else {
			//今のステータス
			frontState_ = state_;
			//弾の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			//ダメージ状態に遷移する
			change_state(State::Damage);
		}
		return;
	}
	//プレイヤーまたは敵に衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

//Ai側からのステータス変更
void Tank::ChangeState(Tank::State state) {

	switch (state) {

	case Tank::State::Idle:
		change_state(State::Idle);
		break;
	case Tank::State::Move:
		change_state(State::Move);
		break;
	case Tank::State::Attack:
		change_state(State::Attack);
		break;
	case Tank::State::Damage:
		change_state(State::Damage);
		break;
	case Tank::State::RunAway:
		change_state(State::RunAway);
		break;
	case Tank::State::Die:
		change_state(State::Die);
		break;
	}
}

//Ai側に現在のステータスを返す
Tank::State Tank::StateNow() {
	return state_;
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
void Tank::change_state(State state) {

	//状態の更新
	state_ = state;
	//状態タイマの初期化
	state_timer_ = 0.f;
}

//アイドル状態
void Tank::idle(float delta_time) {
	//何もなければ、アイドル状態のまま
	change_state(State::Idle);
}

//移動
void Tank::move(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (turnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -turnAngle_, turnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

	GSvector3 moveto = Destination - transform_.position();
	transform_.translate(moveto.normalized() * walkSpeed_ * delta_time, GStransform::Space::World);

	//目標地点に到達したら攻撃開始
	if (target_distance() <= 1.5f) 	change_state(State::Attack);
}

//攻撃
void Tank::attack(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle_fire();
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (turnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -turnAngle_, turnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

	if (aiAttackFrag_) {
		attackTime_ -= delta_time;
		if (attackTime_ <= 0) {
			generate_bullet();
			attackTime_ = 30.0f;
			tankBullet_--;
		}
		if (tankBullet_ <= 0) {
			aiAttackFrag_ = false;
			aiAfterAttackFrag_ = true;
		}
	}
}

//残弾の初期化
void Tank::SetBullet() {
	tankBullet_ = assignmentTankBullet_;
}

void Tank::setattackfrag(bool frag) {
	aiAttackFrag_ = frag;
}

bool Tank::attackfrag() const {
	return aiAttackFrag_;
}

void Tank::setafterattackfrag(bool frag) {
	aiAfterAttackFrag_ = frag;
}

bool Tank::afterattackfrag()const {
	return aiAfterAttackFrag_;
}

void Tank::AttackPoint(GSvector3 pos) {
	Destination = pos;
}

//ダメージ
void Tank::damage(float delta_time) {

	//ノックバックする
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	velocity_ -= GSvector3{ velocity_.x,0.f,velocity_.z }*0.5f * delta_time;

	change_state(frontState_);
}

//退却
void Tank::runaway(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle();
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (turnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -turnAngle_, turnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

	GSvector3 moveto = Destination - transform_.position();
	transform_.translate(moveto.normalized() * walkSpeed_ * delta_time, GStransform::Space::World);

	//目標地点に到達したら死亡状態にする
	if (target_distance() <= 1.5f) {
		//撤退フラグを上げる
		runAwayFrag_ = true;
		tag_ = "DieEnemyTag";
		change_state(State::Die);
		drawMeshFrag_ = false;
	}
}

//死亡
void Tank::Die(float delta_time) {

	//撤退による死でない時
	if (!runAwayFrag_) {

		//爆発エフェクト再生していなかったら
		if (!playExplosionEffect_) {

			//プレイヤーと近かったら鳴らす
			if (playEffectDistance_)gsPlaySE(SE_DieExplosion);

			playExplosionEffect_ = true;
			//爆発エフェクトをその場で再生
			effectExplosionL_ = gsPlayEffect(Effect_ExplosionL, &mypos_);
			drawMeshFrag_ = false;
		}

		//爆発エフェクトの再生が終了したらタグ変更
		if (!gsExistsEffect(effectExplosionL_))	tag_ = "DieEnemyTag";
	}
}

//弾生成
void Tank::generate_bullet() {

	GSvector3 pos = transform_.position();

	pos.y += generateBulletOffsetY_;

	GSvector3 playerpos = player_->transform().position();

	playerpos.y += generateBulletOffsetY_;

	GSvector3 velocity = (playerpos - pos).normalized();

	world_->add_actor(new TankBullet{ world_,pos,velocity ,attackValue_ });
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
	line.end = position + GSvector3{ 0.f,-footOffset_,0.f };
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