#include "EnemyAttackRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "GSeffect.h"

EnemyAttackRange::EnemyAttackRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {

	world_ = world;

	tag_ = "EnemyBulletTag";

	name_ = "EnemyAttackRange";

	velocity_ = velocity;

	collider_ = BoundingSphere{ 2.0f };

	transform_.position(position);

	lifespanTimer_ = 60.0f;

	m_AttackValue = Damage;
	//エフェクト
	effectHandle_ = gsPlayEffect(Effect_SlashGray, &position);
}

void EnemyAttackRange::update(float delta_time) {

	//寿命が尽きたら死亡
	if (lifespanTimer_ <= 0.f) {
		die();
		return;
	}
	//寿命の更新
	lifespanTimer_ -= delta_time;
}

void EnemyAttackRange::draw() const {
	collider().draw();
}

void EnemyAttackRange::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		tag_ = "DieTag";
	}
}