#include "CollisionDetection.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"


CollisionDerection::CollisionDerection(IWorld* world, const GSvector3& position, std::string target, float radius) {

	world_ = world;

	tag_ = "CollisionDerectionTag";
	name_ = "CollisionDerection";

	//大きさ
	collider_ = BoundingSphere{ radius };

	transform_.position(position);

	//対象のタグ
	target_ = target;

}

void CollisionDerection::draw() const {
	collider().draw();
}

void CollisionDerection::react(Actor& other) {

	if (other.tag() == target_) {
		setFrag(true);
	}

}

bool CollisionDerection::Frag() {
	return frag_;
}

//フラグの変更
void CollisionDerection::setFrag(bool frag) {

	frag_ = frag;
}
