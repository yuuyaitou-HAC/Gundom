#include "UnderBoss.h"
#include "UnderBoss.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "Collision/BasicAttackCollider.h"
#include "UnderBossBullet/UnderBossAttackRange.h"
#include <GSmathf.h>
#include "BattleShip/EnemyShip.h"
#include "Common/GameData.h"
#include "Collision/Ray.h"
#include "GSeffect.h"


//アニメーション
enum {
	//アイドルモーション
	Motion_Idle_G = 0,

	//銃装備時の移動
	Motion_WarkF_G = 3,

	//銃装備時の移動攻撃
	Motion_MAttackF_G = 15,
	Motion_MAttackB_G = 16,
	Motion_MAttackL_G = 17,
	Motion_MAttackR_G = 18,

	//銃装備時の空中高速移動
	Motion_RunF_GunAir = 23,

	//銃装備時のその場での攻撃
	Motion_Attack_GunEarth = 30,

	//剣装備時の攻撃(コンボ含む)
	Motion_Attack1_SubarEath = 33,

	//銃装備時に死んだ
	Motion_Die_GunEarth = 49,
};

UnderBoss::UnderBoss(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_underBoss,Mesh_underBoss ,Mesh_underBoss,1,true },
	motion_{ Motion_Idle_G }
{
	world_ = world;
	tag_ = "UnderBossTag";
	name_ = "UnderBoss";
	collider_ = BoundingSphere{ underBossRadius_,GSvector3{0.f,underBossHeight_,0.f} };
	transform_.position(position);
	mesh_.Transform(transform_.localToWorldMatrix());

	//ボスステータスを生成
	underbossstate_ = new UnderBossState();

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//ボス弾管理クラスを生成
	GC_ = new UnderBossGunController{ world_,transform_.position() };

	//敵戦艦取得
	enemyship_ = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));

	//初期化
	underbossstate_->initialize_state_();
}

UnderBoss::~UnderBoss() {
	//ボスで生成したものを削除
	delete GC_;
	delete underbossstate_;
}

void UnderBoss::update(float delta_time) {

	//移動速度
	walkSpeed_ = underbossstate_->MoveSpeed();

	//最大速度
	maxSpeed_ = walkSpeed_ * 5;

	//慣性用のスピード変数を一定値内にとどめる
	speed_ = CLAMP(speed_, 0, maxSpeed_);

	//状態の更新
	update_state(delta_time);

	if (!isFry_) {
		//重力の更新
		velocity_.y += gravity_ * delta_time;
		//重力を加える
		transform_.translate(0.f, velocity_.y, 0.f);
	}
	else velocity_.y = 0;

	//フィールドとの当たり判定
	collide_field();

	//モーションの変更
	mesh_.ChangeMotion(motion_, motionLoop_);

	//メッシュのモーションを更新
	mesh_.Update(delta_time);

	//ワールド変換行列を設定
	mesh_.Transform(transform_.localToWorldMatrix());

	//自身の座標を取得
	myPos_ = transform_.position();

	//プレイヤーの座標を取得
	playerPos_ = player_->transform().position();

	//ボス弾管理クラスのアップデートを呼ぶ
	GC_->update(delta_time);

	//一定距離プレイヤーと近づいたら斬撃を放つ
	if (target_distance(playerPos_, myPos_) <= 2) {
		change_state(UnderBoss::State::Slashing, Motion_Attack1_SubarEath);
	}

	//距離に応じて銃を切り替える
	changeGun();

	//飛ぶかどうかの判定
	changeFly();

	//無敵時間
	if (damageFrag_) {
		invincibleTimer_ -= delta_time;
		if (invincibleTimer_ <= 0) {
			invincibleTimer_ = assignmnetInvincibleTimer_;
			damageFrag_ = false;
			meshAlpha_ = 1.0f;
		}
	}
}

void UnderBoss::draw() const {

	if (drawmeshFrag_) {
		float transparency = gsGetDitheredTransparency();
		GScolor current_color;
		glGetFloatv(GL_CURRENT_COLOR, current_color);
		GScolor current_secondary_color;
		glGetFloatv(GL_CURRENT_SECONDARY_COLOR, current_secondary_color);
		gsSetDitheredTransparency(meshAlpha_);
		//メッシュの描画
		mesh_.Draw();
		GC_->draw();
		//武器を描画
		gsSetDitheredTransparency(transparency);
		glColor4fv(current_color);
		glSecondaryColor3fv(current_secondary_color);
	}
}

void UnderBoss::react(Actor& other) {

	//ダメージ中またはダウン中の場合は何もしない
	if (state_ == State::Damage || state_ == State::Die)return;
	//プレーヤーの弾に衝突した
	if (other.tag() == "PlayerBulletTag" && !damageFrag_) {

		//ダメージを受け取る関数
		damage_ = static_cast<BasicAttackCollider*>(&other)->GetAttackValue() - underbossstate_->Defense();

		if (damage_ <= 0)damage_ = 0;

		//体力を減らす
		underbossstate_->AddHP(-damage_);

		//ダメージSE
		gsPlaySE(SE_Damage1);

		if (underbossstate_->HP() <= 0) {

			//残りの体力がなければダウン状態に遷移
			change_state(State::Baster, Motion_Die_GunEarth, false);

		}
		else {
			//弾の進行方向にノックバックする移動量を求める
			velocity_ = other.velocity().getNormalized() * 0.5f;
			damageFrag_ = true;
			meshAlpha_ = 0.5f;
			state_ = UnderBoss::State::Damage;
		}
		return;
	}
	//プレイヤーまたは敵に衝突した
	if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

UnderBossState* UnderBoss::underBossState_() const {
	return underbossstate_;
}

//銃の切り替え
void UnderBoss::changeGun() {
	//プレイヤーとの距離
	float distance = target_distance(playerPos_, myPos_);

	//銃撃
	if (distance >= weaponDistance_) {

		//銃の種類の変更(ビームライフル)してステータスを攻撃にする
		GC_->SetState(1);
		//ボスステータスの変更
		underBossState_()->SetGunState(UnderBossState::GunState::Beamlifl);
	}
	else {
		//銃の種類の変更(ガトリング)してステータスを攻撃にする
		GC_->SetState(2);
		//ボスステータスの変更
		underBossState_()->SetGunState(UnderBossState::GunState::Gatling);
	}
}

void UnderBoss::update_state(float delta_time) {

	switch (state_)
	{
	case UnderBoss::State::Move:
		move(delta_time);
		break;
	case UnderBoss::State::AttackMove:
		attackMove(delta_time);
		break;
	case UnderBoss::State::Slashing:
		slash(delta_time);
		break;
	case UnderBoss::State::Damage:
		damage(delta_time);
		break;
	case UnderBoss::State::Baster:
		baster(delta_time);
		break;
	case UnderBoss::State::Retreat:
		retreat(delta_time);
		break;
	case UnderBoss::State::Die:
		death(delta_time);
		break;
	}

	stateTimer_ += delta_time;
}

void UnderBoss::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motionLoop_ = loop;
	state_ = state;
	stateTimer_ = 0.f;
}

//移動処理
void UnderBoss::move(float delta_time) {

	//プレイヤーに方向を
	// 向かせる
	faceThePlayer(delta_time);

	//前進する（ローカル座標）
	transform_.translate(0.f, 0.f, walkSpeed_ * delta_time);

	//プレイヤーと一定距離近づいたら
	if (target_distance(playerPos_, myPos_) < 25) {

		//プレイヤー方向のベクトルを取得
		postmoveTo_ = player_->transform().position().normalized();

		//その場で攻撃開始
		change_state(UnderBoss::State::AttackMove, Motion_Attack_GunEarth);
	}
}

//飛ぶかどうか
void UnderBoss::changeFly() {
	//プレイヤーが自身より上にいる
	//プレイヤーの高さがジャンプの範疇を超えたとき
	if (playerPos_.y > 3)isFry_ = true;
	else isFry_ = false;
}

void UnderBoss::attackMove(float delta_time) {

	//プレイヤーの方向を向かせる
	faceThePlayer(delta_time);

	//移動方向ベクトル更新までの時間
	moveTimer_ -= delta_time;

	//一定時間で目標地点更新
	if (moveTimer_ <= 0) {

		if (!movein_) {
			//ランダムな方向ベクトルを取得
			attackPoint_ = attackPoint();

			//2つのベクトルの内積を求める
			double Dot = GSvector3::dot(postmoveTo_, attackPoint_);

			//それぞれのベクトルの長さを取得
			double magA = postmoveTo_.magnitude();
			double magB = attackPoint_.magnitude();

			double cosTheta = Dot / (magA * magB);

			cosTheta = std::max(-1.0, std::min(1.0, cosTheta));

			float ragian = std::acos(cosTheta);

			//減少率
			reductionRate_ = (1 - cos(ragian)) / 2;

			//減少値 
			//180度でspeedを０に
			//0度で減少無し
			reduceSpeed_ = walkSpeed_ - (reductionRate_ * walkSpeed_);

			speed_ = walkSpeed_;

			movein_ = true;
		}

		//増減率
		float moveReduction = 0.01f;

		if (!fluctuation_) {

			//スピードを徐々に減らしていく
			speed_ -= delta_time * moveReduction;

			//減少値まで減少したらフラグを変える
			if (speed_ <= reduceSpeed_) 	fluctuation_ = true;
		}
		else {
			//過去と向かう方向ベクトルの更新
			postmoveTo_ = attackPoint_;
			moveTo_ = attackPoint_;
			//徐々にスピードを上げる
			speed_ += delta_time * moveReduction;
		}

		//元のスピードになったら時間の初期化
		if (speed_ >= walkSpeed_) {
			//時間の初期化
			moveTimer_ = assignmentMoveTimer_;

			fluctuation_ = false;

			movein_ = false;
		}
	}

	//向かう方向
	transform_.translate(moveTo_ * speed_ * delta_time);

	//モーション番号
	GSint motion;
	//自身の前と目標地点との角度差
	float angle = GSvector3::signedAngle(transform_.forward(), moveTo_);

	if (!isFry_) {
		if (0 < angle) {
			if (angle < 80)motion = Motion_MAttackF_G;
			else if (angle <= 100)motion = Motion_MAttackR_G;
			else motion = Motion_MAttackB_G;
		}
		else {
			if (-80 < angle)motion = Motion_MAttackF_G;
			else if (-100 <= angle)motion = Motion_MAttackL_G;
			else motion = Motion_MAttackB_G;
		}
	}

	change_state(UnderBoss::State::AttackMove, 1);

	//弾を撃つ処理
	shoot(delta_time);

	//一定距離離れたら
	if (target_distance(playerPos_, myPos_) >= 50) {
		change_state(UnderBoss::State::Move, Motion_WarkF_G);
	}
}

GSvector3 UnderBoss::attackPoint() {

	if (isFry_)point_ = GSvector3{ (float)gsRand(-30,30),(float)gsRand(0,10) + playerPos_.y,(float)gsRand(-30,30) };
	else point_ = GSvector3{ (float)gsRand(-30,30),0,(float)gsRand(-30,30) };

	point_ = (point_ - myPos_).normalized();

	//マイナス要素を加える
	point_.x *= sign();
	point_.y *= sign();
	point_.z *= sign();

	return point_;
}

//飛
void UnderBoss::fry(float delta_time) {

	fryTimer_ -= delta_time;

	// 高さの設定
	if (fryTimer_ < 0) {
		fryPow_.y = gsRand(fryRand_.x, fryRand_.y) + playerPos_.y;

		fryTimer_ = assignmentFryTimer_;
	}

	// 目標地点への加速度を計算
	float distance = fryPow_.y - myPos_.y;
	float acceleration = distance * 0.1f; // 距離に比例した加速度（調整可能な係数）
	velocity_.y += acceleration * delta_time;

	// 最大速度を制限
	const float MaxSpeed = 5.0f; // 調整可能
	if (velocity_.y > MaxSpeed) {
		velocity_.y = MaxSpeed;
	}
	if (velocity_.y < -MaxSpeed) {
		velocity_.y = -MaxSpeed;
	}

	// 現在位置を更新
	transform_.translate(0, velocity_.y * walkSpeed_, 0);

	// 目標の高さ到達後に速度を減衰
	if (std::abs(distance) < 1.0f) { // 1.0f は目標地点の許容誤差
		velocity_.y *= 0.9f;         // 減衰係数（0.9fなど）
		if (std::abs(velocity_.y) < 0.01f) {
			velocity_.y = 0.0f;      // 微小な速度をゼロに
		}
	}
}

//目標地点がざひょうじょうにいるかどうか
bool UnderBoss::onTheLine(GSvector3 point)const {
	double x1 = playerPos_.x;
	double y1 = playerPos_.y;
	double z1 = playerPos_.z;
	double x2 = myPos_.x;
	double y2 = myPos_.y;
	double z2 = myPos_.z;
	double xc = point.x;
	double yc = point.y;
	double zc = point.z;

	// 同一直線上にあるか (比が一致するかを確認)
	double t1 = (x2 - x1) != 0 ? (xc - x1) / (x2 - x1) : 0;
	double t2 = (y2 - y1) != 0 ? (yc - y1) / (y2 - y1) : 0;
	double t3 = (z2 - z1) != 0 ? (zc - z1) / (z2 - z1) : 0;

	// 比がほぼ同じかを確認
	if (fabs(t1 - t2) > EPSILON_ || fabs(t2 - t3) > EPSILON_ ||
		fabs(t1 - t3) > EPSILON_) return false;

	// 線分上にあるか
	if (MIN(x1, x2) <= xc && xc <= MAX(x1, x2) &&
		MIN(y1, y2) <= yc && yc <= MAX(y1, y2) &&
		MIN(z1, z2) <= zc && zc <= MAX(z1, z2)) {
		return true;
	}

	return false;
}

void UnderBoss::shoot(float delta_time) {

	shootTime_ -= delta_time;
	//銃の種類がビームライフルなら
	if (underBossState_()->gunstate_() == UnderBossState::GunState::Beamlifl) {
		//残弾があり一定時間たったら
		if (underBossState_()->BeamBullet() > 0 && shootTime_ <= 0) {
			GC_->Fire();
			shootTime_ = assignmentBeamShootTime_;
		}
	}
	//銃の種類がガトリングなら
	else if (underBossState_()->gunstate_() == UnderBossState::GunState::Gatling) {

		if (underBossState_()->GatlingBullet() > 0 && shootTime_ <= 0) {
			GC_->Fire();
			shootTime_ = assignmentGatringShootTime_;
		}
	}
}

//バスターライフル発射
void UnderBoss::baster(float delta_time) {

	basterTimer_ -= delta_time;

	if (basterTimer_ <= 0) {
		//銃の種類の変更(ビームライフル)してステータスを攻撃にする
		GC_->SetState(3);
		//ボスステータスの変更
		underBossState_()->SetGunState(UnderBossState::GunState::Basterlifl);
		GC_->Fire();
		change_state(State::Die, Motion_Die_GunEarth);
	}
}

//斬撃
void UnderBoss::slash(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle(playerPos_);
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

	GSvector3 pos = myPos_ + transform_.forward() * slashDistance_;
	pos.y += slashHight_;

	//斬撃の生成
	world_->add_actor(new UnderBossAttackRange{ world_,pos,GSvector3().zero(),slashValue_ });

	afterAlash();
}

//自身の後ろに後退
void UnderBoss::afterAlash() {

	rotate_ = transform_.forward();

	rotate_.y += 0.2f;

	GSvector3 bossrotate = rotate_.normalize();

	GSvector3 br = bossrotate * 0.4f;

	for (int i = 0; i < 10; i++) {
		transform_.translate(-br, GStransform::Space::World);
	}
	change_state(UnderBoss::State::Move, Motion_WarkF_G);
}

//退却
void UnderBoss::retreat(float delta_time) {
	//戦艦の座標取得
	GSvector3 shippos = enemyship_->transform().position();

	Ray ray = { enemyship_->transform().position(),-(transform_.up()) };
	GSvector3 intersect;
	world_->field()->collide(ray, enemyship_->transform().position().y + 30.0f, &intersect);

	shippos.y = intersect.y;

	//ターゲット方向の角度を求める
	float angle = target_signed_angle(shippos);
	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (turnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -turnAngle_, turnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);

	GSvector3 moveto = shippos - transform_.position();
	transform_.translate(moveto.normalized() * walkSpeed_ * 1.5f * delta_time, GStransform::Space::World);

	//目標地点に到達したら死亡状態にする
	if (target_distance(myPos_, shippos) <= 1.5f) {
		change_state(State::Die, 0);
	}
}

void UnderBoss::damage(float delta_time) {
	//ヒットエフェクト再生
	effectHit_ = gsPlayEffect(Effect_Hit, &myPos_);

	//アニメーション再生後移動攻撃にステータス変更
	if (gsExistsEffect(effectHit_)) {
		change_state(State::Move, Motion_WarkF_G);
	}
}

void UnderBoss::death(float delta_time) {

	if (playExplosionEffect_) {
		playExplosionEffect_ = true;

		//高さ調整
		GSvector3 pos = myPos_;
		pos.y += 2.0f;
		gsPlaySE(SE_BossDieExplosion);
		effectExplosionL_ = gsPlayEffect(Effect_ExplosionL, &pos);
		drawmeshFrag_ = false;
	}
	if (!gsExistsEffect(effectExplosionL_)) {
		//ゲームに自身の死を知らせる
		world_->gameData()->setUnderBossDie(true);
		die();
	}
}

void UnderBoss::faceThePlayer(float delta_time) {

	//ターゲット方向の角度を求める
	float angle = target_signed_angle(playerPos_);

	//振り向き角度よりも角度の差があるか？
	if (std::abs(angle) > (turnAngle_ * delta_time)) {
		//角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
		angle = CLAMP(angle, -turnAngle_, turnAngle_) * delta_time;
	}
	//向きを変える
	transform_.rotate(0.f, angle, 0.f);
}

//ターゲットと自身のなす角度
float UnderBoss::target_signed_angle(GSvector3 target) {

	//プレイヤーと自身の座標の方向ベクトル
	GSvector3 to_target = target - myPos_;

	GSvector3 forward = transform_.forward();

	to_target.y = 0;
	forward.y = 0;

	return GSvector3::signedAngle(forward, to_target);
}

//2点間の距離
float UnderBoss::target_distance(GSvector3 Targetpos, GSvector3 pos) {
	return GSvector3::distance(Targetpos, pos);
}

void UnderBoss::collide_actor(Actor& other) {

	//y座標を除く座標を求める
	GSvector3 position = myPos_;
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

int UnderBoss::sign() {
	int num = gsRand(-1, 1);

	if (num == 1 || num == -1)return num;
	else return sign();
}

void UnderBoss::collide_field() {
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
	line.end = position + GSvector3{ 0.f,-footOffset_,0.f };
	GSvector3 intersect;//地面との交点
	if (world_->field()->collide(line, &intersect)) {
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.f;
		isFry_ = false;
	}
}