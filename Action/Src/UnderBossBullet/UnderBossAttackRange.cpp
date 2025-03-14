#include "UnderBossAttackRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

UnderBossAttackRange::UnderBossAttackRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage){

	world_ = world;

	tag_ = "EnemyBulletTag";

	name_ = "BossAttackRange";

	velocity_ = velocity;

	collider_ = BoundingSphere{ 1.0f };

	transform_.position(position);

	lifespan = 30.0f;

	m_AttackValue = Damage;

}

void UnderBossAttackRange::update(float delta_time)
{
	if (lifespan <= 0) {
		die();
		return;
	}

	lifespan -= delta_time;

}

void UnderBossAttackRange::draw() const
{
	//collider().draw();

}

void UnderBossAttackRange::react(Actor& other){

	if (other.tag() == "PlayerTag") {
		die();
	}
}
