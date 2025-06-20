#include "AllRangeUnit.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "PlayerBullet/PlayerBullet.h"
#include "GSeffect.h"

AllRangeUnit::AllRangeUnit(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_AllRangeUnit,Mesh_AllRangeUnit ,Mesh_AllRangeUnit ,0 },
	state_{ State::Sortie } {

	world_ = world;

	tag_ = "AllRangeUnitTag";

	name_ = "AllRangeUnit";

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//生成時にプレイヤーの方向を向かせる
	transform_.rotate(player_->transform().rotation());

	//自身のy軸を取得
	posY_ = transform_.position().y;

	//バーニアエフェクト
	vernierEffect_ = gsPlayEffect(Effect_VernierBL, &position);
}

AllRangeUnit::~AllRangeUnit() {
	//エフェクトの停止
	gsStopEffect(vernierEffect_);
	die();
}

void AllRangeUnit::update(float delta_time) {

	pos_ = transform_.position();

	update_state(delta_time);

	//メッシュを更新
	mesh_.Update(delta_time);

	mesh_.Transform(transform_.localToWorldMatrix());

	local_matrix = GSmatrix4::TRS(effectPos_, GSquaternion::euler(effectEuler_), effectScale_);
	world = local_matrix * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernierEffect_, &world);
}

void AllRangeUnit::draw() const {
	mesh_.Draw();
}

void AllRangeUnit::update_state(float delta_time) {

	switch (state_)
	{
	case AllRangeUnit::State::Sortie:
		sortie(delta_time);
		break;
	case AllRangeUnit::State::Attack:
		attack(delta_time);
		break;
	case AllRangeUnit::State::Retreat:
		retreat(delta_time);
		break;
	case AllRangeUnit::State::Deth:
		deth(delta_time);
		break;
	}
}

//生成時
void AllRangeUnit::sortie(float delta_time) {

	velocity_ = pos_.up() * speed_;

	//自身の上方向に移動
	transform_.translate(velocity_ * delta_time);

	if (pos_.y - posY_ >= 5)change_state(State::Attack);
}

//攻撃
void AllRangeUnit::attack(float delta_time) {
	//対象の有無で行動を変える
	if (target_ == NULL)toPlayer(delta_time);
	else to_target(delta_time);
}

//プレイヤーに追従
void AllRangeUnit::toPlayer(float delta_time) {

	//ランダム座標を出していないかつ止まっていない時
	if (!randPosTrigger_ && velocity_ != GSvector3::zero()) {
		//前後左右のランダムな座標を取得
		randRL_ = gsRand(randRLRange_.x, randRLRange_.y);
		randUD_ = gsRand(randUDRange_.x, randUDRange_.y);
		randPosTrigger_ = true;
	}

	//プレイヤーの座標
	GSvector3 playerpos = player_->transform().position();
	playerpos.y += playerOffsetY_;

	//出したランダム座標をプレイヤーの座標と合わせる
	playerpos += player_->transform().localToWorldMatrix().left() * randRL_;
	playerpos += player_->transform().localToWorldMatrix().up() * randUD_;

	//プレイヤーの速度
	float playerspeed = player_->player_state()->moveSpeed() * speedMagnification_;

	//プレイヤーがダッシュ時にはさらに速度を上げる
	if (gsGetKeyState(GKEY_LSHIFT))playerspeed *= speedMagnification_;

	//距離に応じて処理を変える
	if (GSvector3::distance(playerpos, pos_) <= 2) {

		velocity_ = GSvector3::zero();

		transform_.translate(velocity_);

		//自身のクォータニオン
		GSquaternion myquaternion = transform_.rotation();

		//プレイヤーのクォータニオン
		GSquaternion playerquaternion = player_->transform().rotation();

		GSquaternion myToplayer = GSquaternion::slerp(myquaternion, playerquaternion, delta_time * 0.1);

		//プレイヤーと同じ方向を向く
		transform_.rotation(myToplayer);
		randPosTrigger_ = false;
	}
	else
	{
		//プレイヤー方向のベクトル
		GSvector3 directionToPlayer = playerpos - pos_;

		GSquaternion targetRotation = GSquaternion::lookRotation(directionToPlayer);

		//自身のクォータニオン
		GSquaternion currentRotation = transform_.rotation();

		//補完
		GSquaternion newRotation = GSquaternion::slerp(currentRotation, targetRotation, delta_time * 0.1f);
		transform_.rotation(newRotation);

		velocity_.z = playerspeed;
		velocity_.x = velocity_.y = 0;
		transform_.translate(velocity_ * delta_time);
	}
}

//ターゲットに攻撃
void AllRangeUnit::to_target(float delta_time) {

	//対象との距離
	GSvector3 targetpos = target_->transform().position();
	targetpos.y += targetOffsetY_;

	//敵のタグが取得時と異なったもしくは一定距離離れたら対象から外す
	if (target_->tag() != "EnemyTag" || GSvector3::distance(targetpos, player_->transform().position()) > targetDistance_) {
		target_ = NULL;
		moveFlag_ = false;
		return;
	}

	//ランダムな座標取得
	if (!moveFlag_) {

		randPos_ = rand_position();
		moveFlag_ = true;
	}

	float distance = GSvector3::distance(pos_, randPos_);

	//目標地点との差が一定数以下になったら
	if (distance < 5.0f) {

		//移動量を0にする
		targetToVelocity_ = GSvector3::zero();

		//弾生成
		generate_bullet();
		moveFlag_ = false;

		return;
	}

	targetToVelocity_ = randPos_ - pos_;

	velocity_ = targetToVelocity_.normalized() * speed_;

	//移動
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

	//対象の方向を向かせる
	GSvector3 look = targetpos - pos_;
	GSquaternion lookrotation = GSquaternion::lookRotation(look);
	transform_.rotation(lookrotation);
}

GSvector3 AllRangeUnit::rand_position() {

	//ターゲットの座標取得
	GSvector3 targetpos = target_->transform().position();

	//高さ調整
	targetpos.y += targetOffsetY_;

	//ターゲットを中心にランダムな座標を生成
	GSvector3 randampos = GSvector3{ (float)gsRand(randTargetXRange_.x,randTargetXRange_.y)
		+ targetpos.x,(float)gsRand(randTargetYRange_.x,randTargetYRange_.y)
		+ targetpos.y,(float)gsRand(randTargetZRange_.x,randTargetZRange_.y) + targetpos.z };

	//ターゲットとランダム座標の間を出す
	float distance = GSvector3::distance(targetpos, randampos);

	//条件内なら座標を返す
	if (distance < targetRandDistance.y &&
		distance > targetRandDistance.x) {

		return randampos;
	}
	//条件に当てはまらなかったらもう一度この関数の処理を行う
	return rand_position();
}

//弾生成
void AllRangeUnit::generate_bullet() {

	//生成座標
	GSvector3 position = pos_ + transform_.forward();

	//移動量
	GSvector3 velocity = transform_.forward() * 1.5f;

	//攻撃力
	int attackvalue = player_->player_state()->attack() * 0.5f;

	//弾生成
	world_->add_actor(new PlayerBullet{ world_,position,velocity,attackvalue,"AllRangeBullet" });
}

//退却
void AllRangeUnit::retreat(float delta_time) {

	//プレイヤーの方向を向かせる
	transform_.lookAt(player_->transform());

	//プレイヤーに向かう方向ベクトル
	GSvector3 playerPos = player_->transform().position() - transform_.position();

	//プレイヤーと自身の間
	float distance = GSvector3::distance(player_->transform().position(), transform_.position());

	//慣性
	float speedvalue = returnSpeed_ * distance * 0.1;
	speedvalue = CLAMP(speedvalue, 0, returnSpeed_);

	//移動
	transform_.translate(playerPos.normalized() * speedvalue * delta_time, GStransform::Space::World);

	//一定距離近づいたら死亡
	if (distance <= dieDistance_) {
		change_state(State::Deth);
		dieTrigger_ = true;
	}
}

//死亡処理
void AllRangeUnit::deth(float delta_time) {
	die();
}

//ターゲットとの符号付き角度を返す
float AllRangeUnit::target_signed_angle(GSvector3 target) {

	GSvector3 to_target = target - pos_;
	GSvector3 forward = transform_.forward();

	forward.y = 0.0f;
	to_target.y = 0.0f;

	return GSvector3::signedAngle(forward, to_target);
}

//符号を返す
int AllRangeUnit::sign() {

	int num = gsRand(-1, 1);

	if (num == 1 || num == -1) return num;
	else return sign();
}

//ターゲットをセットする
void AllRangeUnit::set_target(Actor* target) {
	target_ = target;
}

//現在持っているターゲットを返す
const Actor* AllRangeUnit::return_target() const {
	return target_;
}

//現在のステータス取得
AllRangeUnit::State AllRangeUnit::now_state() const {
	return state_;
}

//ステータスの変更
void AllRangeUnit::change_state(AllRangeUnit::State state) {
	if (!dieTrigger_)state_ = state;
}