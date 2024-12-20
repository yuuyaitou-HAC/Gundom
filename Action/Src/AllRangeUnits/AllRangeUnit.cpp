#include "AllRangeUnit.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "PlayerBullet/PlayerBullet.h"

//移動速度
const float MoveSpeed = 1.5f;

const float speed = 0.5f;

const float TurnAngle = 2.5f;

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
	posy = transform_.position().y;
}

AllRangeUnit::~AllRangeUnit() {
	die();
}

void AllRangeUnit::update(float delta_time) {

	pos = transform_.position();

	//メッシュを更新
	mesh_.Update(delta_time);

	mesh_.Transform(transform_.localToWorldMatrix());

	update_state(delta_time);
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

	state_timer += delta_time;
}

void AllRangeUnit::change_state(State state) {
	state_ = state;
	state_timer = 0;
}

//生成時
void AllRangeUnit::sortie(float delta_time) {

	//自身の上方向に移動
	transform_.translate(pos.up() * speed * delta_time);

	if (pos.y - posy >= 5) {
		change_state(State::Attack);
	}
}

//攻撃
void AllRangeUnit::attack(float delta_time) {
	//対象がいなかったらプレイヤーに追従
	if (target_ == NULL) {
		toPlayer(delta_time);
	}
	else {//対象がいたら対象に対して攻撃
		toTarget(delta_time);
	}

	//弾の生成
	if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_2)) {
		generate_bullet();
	}
}

//プレイヤーに追従
void AllRangeUnit::toPlayer(float delta_time) {

	if (!randpos && velocity_ != GSvector3::zero()) {
		randRL = gsRand(-3, 3);
		randUD = gsRand(-1, 1);
		randpos = true;
	}

	//プレイヤーの座標
	GSvector3 playerpos = player_->transform().position();
	playerpos.y += 3.0f;

	playerpos += player_->transform().localToWorldMatrix().left() * randRL;
	playerpos += player_->transform().localToWorldMatrix().up() * randUD;

	float playerspeed = player_->playerState_()->moveSpeed() * 1.5;

	if (gsGetKeyState(GKEY_LSHIFT)) {
		playerspeed *= 1.5;
	}

	//距離に応じて処理を変える
	if (GSvector3::distance(playerpos, transform_.position()) <= 2) {

		velocity_ = GSvector3::zero();

		transform_.translate(velocity_);

		//自身のクォータニオン
		GSquaternion myquaternion = transform_.rotation();

		//プレイヤーのクォータニオン
		GSquaternion playerquaternion = player_->transform().rotation();

		GSquaternion myToplayer = GSquaternion::slerp(myquaternion, playerquaternion, delta_time * 0.1);

		//プレイヤーと同じ方向を向く
		transform_.rotation(myToplayer);
		randpos = false;
	}
	else
	{
		//プレイヤー方向のベクトル
		GSvector3 directionToPlayer = playerpos - transform_.position();

		GSquaternion targetRotation = GSquaternion::lookRotation(directionToPlayer);

		//自身のクォータニオン
		GSquaternion currentRotation = transform_.rotation();

		//補完
		GSquaternion newRotation = GSquaternion::slerp(currentRotation, targetRotation, delta_time * 0.1f);
		transform_.rotation(newRotation);

		velocity_.z = playerspeed * delta_time;
		transform_.translate(0, 0, velocity_.z);
	}
}

//ターゲットに攻撃
void AllRangeUnit::toTarget(float delta_time) {

	//対象との距離
	GSvector3 targetpos = target_->transform().position();
	targetpos.y += 1.0f;

	//敵のタグが取得時と異なったもしくは一定距離離れたら対象から外す
	if (target_->tag() != "EnemyTag" || GSvector3::distance(targetpos, player_->transform().position()) > 20) {
		target_ = NULL;
	}

	//対象の方向を向かせる
	transform_.lookAt(targetpos);
}

//弾生成
void AllRangeUnit::generate_bullet() {

	GSvector3 position = pos + transform_.forward();

	GSvector3 velocity = transform_.forward() * 1.5f;

	int attackvalue = player_->playerState_()->attack() * 0.5f;

	world_->add_actor(new PlayerBullet{ world_,position,velocity,attackvalue });
}

//退却
void AllRangeUnit::retreat(float delta_time) {

	transform_.lookAt(player_->transform());

	//プレイヤーに向かう方向ベクトル
	GSvector3 ppos = player_->transform().position() - pos;

	//プレイヤーと自身の間
	float distance = GSvector3::distance(player_->transform().position(), pos);

	//慣性
	float speedvalue = speed * distance * 0.1;
	speedvalue = CLAMP(speedvalue, 0, speed);

	transform_.translate(ppos.normalized() * speedvalue * delta_time);

	if (distance <= 2)change_state(State::Deth);
}

void AllRangeUnit::deth(float delta_time) {

}

float AllRangeUnit::target_signed_angle() {

	GSvector3 to_target = player_->transform().position() - transform_.position();
	GSvector3 forward = transform_.forward();

	forward.y = 0.0f;
	to_target.y = 0.0f;

	return GSvector3::signedAngle(forward, to_target);
}

void AllRangeUnit::settarget(Actor* target) {
	target_ = target;
}

Actor* AllRangeUnit::retuntarget()
{
	return target_;
}

//現在のステータス取得
AllRangeUnit::State AllRangeUnit::nowstate() {

	return state_;
}

//ステータスの変更
void AllRangeUnit::changestate(AllRangeUnit::State state) {

	state_ = state;
}
