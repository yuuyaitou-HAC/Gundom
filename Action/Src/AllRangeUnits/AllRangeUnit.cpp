#include "AllRangeUnit.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "PlayerBullet/PlayerBullet.h"

//移動速度
const float MoveSpeed = 1.5f;

const float speed = 0.5f;

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

	//transform_.lookAt(player_->transform());

	//メッシュを更新
	mesh_.Update(delta_time);

	mesh_.Transform(transform_.localToWorldMatrix());

	update_state(delta_time);

	if (gsGetKeyTrigger(GKEY_0)) {
		change_state(State::Retreat);
	}

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

void AllRangeUnit::sortie(float delta_time) {

	//自身の上方向に移動
	transform_.translate(pos.up() * speed * delta_time);

	if (pos.y - posy >= 5) {
		change_state(State::Attack);
	}
}

void AllRangeUnit::attack(float delta_time) {

	//tamaseisei
	if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_2)) {
		generate_bullet();
	}
}

//弾生成
void AllRangeUnit::generate_bullet() {

	GSvector3 position = pos + transform_.forward();

	GSvector3 velocity = transform_.forward() * 1.5f;

	int attackvalue = player_->playerState_()->attack();

	world_->add_actor(new PlayerBullet{ world_,position,velocity,attackvalue });
}


void AllRangeUnit::retreat(float delta_time) {

	transform_.lookAt(player_->transform());

	//プレイヤーに向かう方向ベクトル
	GSvector3 ppos = player_->transform().position() - pos;

	ppos.y += 1.0f;

	//プレイヤーと自身の間
	float distance = GSvector3::distance(player_->transform().position(), pos);

	//慣性
	float speedvalue = speed * distance * 0.1;
	speedvalue = CLAMP(speedvalue, 0, speed);

	transform_.translate(ppos.normalized() * speedvalue * delta_time);

	if (distance <= 2) {
		change_state(State::Deth);
	}
}

void AllRangeUnit::deth(float delta_time) {

}
