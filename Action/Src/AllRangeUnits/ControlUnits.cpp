#include "ControlUnits.h"
#include "AllRangeUnit.h"
#include "Player/Player.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

const int MakeNumber = 1;

ControlUnits::ControlUnits(IWorld* world, const GSvector3& position) :
	units_{ MakeNumber } {

	world_ = world;

	tag_ = "ControlUnitsTag";

	name_ = "ControlUnits";

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	makeUnits();
}

void ControlUnits::makeUnits() {

	for (int i = 0; i < MakeNumber; i++) {
		units_[i] = new AllRangeUnit{ world_,transform_.position() };
		world_->add_actor(units_[i]);
	}
}

ControlUnits::~ControlUnits() {


}

void ControlUnits::update(float delta_time) {

	if (!Change) {

	}
	else {
		retreat();
	}

}

//“PŽû
void ControlUnits::retreat() {
	for (auto& unit : units_) {
		unit->changestate(AllRangeUnit::State::Retreat);
	}
}

void ControlUnits::changeFrag(bool frag) {
	Change = frag;
}