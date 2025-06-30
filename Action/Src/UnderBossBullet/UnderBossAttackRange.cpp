#include "UnderBossAttackRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

UnderBossAttackRange::UnderBossAttackRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {

	world_ = world;

	tag_ = "EnemyBulletTag";

	name_ = "BossAttackRange";

	velocity_ = velocity;

	collider_ = BoundingSphere{ 1.0f };

	transform_.position(position);

	m_AttackValue = Damage;
}

void UnderBossAttackRange::update(float delta_time)
{
	if (lifeSpan_ <= 0) {
		die();
		return;
	}
	lifeSpan_ -= delta_time;
}

void UnderBossAttackRange::react(Actor& other) {
	if (other.tag() == "PlayerTag")die();
}