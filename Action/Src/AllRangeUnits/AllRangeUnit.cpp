#include "AllRangeUnit.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Player/Player.h"
#include "PlayerBullet/PlayerBullet.h"

AllRangeUnit::AllRangeUnit(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_AllRangeUnit,Mesh_AllRangeUnit ,Mesh_AllRangeUnit ,0 } {

	world_ = world;

	tag_ = "AllRangeUnitTag";

	name_ = "AllRangeUnit";

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//生成時にプレイヤーの方向を向かせる
	transform_.rotate(player_->transform().rotation());

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
		change_state(State::Attack);
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
	}

	state_timer += delta_time;

}

void AllRangeUnit::change_state(State state) {

	state_ = state;
	state_timer = 0;
}

void AllRangeUnit::sortie(float delta_time) {



}

void AllRangeUnit::attack(float delta_time) {

	//tamaseisei
	if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_2)) {
		generate_bullet();
	}

}

void AllRangeUnit::generate_bullet() {

	GSvector3 position = pos + transform_.forward();

	GSvector3 velocity = transform_.forward() * 1.5f;

	int attackvalue = player_->playerState_()->attack();

	world_->add_actor(new PlayerBullet{ world_,position,velocity,attackvalue });
}


void AllRangeUnit::retreat(float delta_time) {


}
