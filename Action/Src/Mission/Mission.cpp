#include "Mission.h"
#include "World/IWorld.h"
#include "Player/Player.h"
#include "BOSS/Boss.h"

Mission::Mission(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "MissionTag";
	name_ = "Mission";

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	boss_ = static_cast<Boss*>(world_->find_actor("boss"));
}


void Mission::update(float delta_time) {

	Mission1(delta_time);

	Mission2(delta_time);

}

void Mission::draw() const {



}

void Mission::Mission1(float delta_time) {

}

void Mission::Mission2(float delta_time) {

}
