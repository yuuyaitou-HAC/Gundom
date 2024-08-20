#include "Shield.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"


Shield::Shield(IWorld* world, const GSvector3 pos) {

	world_ = world;

	tag_ = "ShieldTag";
	name_ = "Shield";

	collider_ = BoundingSphere{ 0.5f,GSvector3{0.0f,0.0f,0.0f} };

	transform_.position(pos);
	
}


void Shield::update(float delta_time) {


}

void Shield::draw() const {


}

void Shield::react(Actor& other) {


}
