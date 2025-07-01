#include "CollisionDetection.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"


CollisionDerection::CollisionDerection(IWorld* world, const GSvector3& position, std::string target, float radius) {

	world_ = world;

	tag_ = "CollisionDerectionTag";
	name_ = "CollisionDerection";

	//‘å‚«‚³
	collider_ = BoundingSphere{ radius };

	transform_.position(position);

	//‘ÎÛ‚Ìƒ^ƒO
	target_ = target;

}

void CollisionDerection::draw() const {
	collider().draw();
}

void CollisionDerection::react(Actor& other) {

	if (other.tag() == target_) {
		//“–‚½‚Á‚½‘Šè‚ğæ“¾
		otherActor_ = static_cast<Actor*>(&other);
	}
}

//“–‚½‚Á‚½‘Šè‚ğ•Ô‚·
Actor* CollisionDerection::actor() {
	return otherActor_;
}