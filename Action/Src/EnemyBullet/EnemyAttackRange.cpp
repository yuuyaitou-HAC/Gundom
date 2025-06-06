#include "EnemyAttackRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

EnemyAttackRange::EnemyAttackRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {

	world_ = world;

	tag_ = "EnemyBulletTag";

	name_ = "EnemyAttackRange";

	velocity_ = velocity;

	collider_ = BoundingSphere{ 2.0f };

	transform_.position(position);

	lifespanTimer_ = 60.0f;

	m_AttackValue = Damage;
}

void EnemyAttackRange::update(float delta_time) {

	//õ–½‚ªs‚«‚½‚ç€–S
	if (lifespanTimer_ <= 0.f) {
		die();
		return;
	}
	//õ–½‚ÌXV
	lifespanTimer_ -= delta_time;
}

void EnemyAttackRange::draw() const {
	collider().draw();
}

void EnemyAttackRange::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}