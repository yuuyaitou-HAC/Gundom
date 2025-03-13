#ifndef ATTACK_RANGE_H_
#define ATTACK_RANGE_H_

#include "Collision/BasicAttackCollider.h"

class AttackRange :public BasicAttackCollider {

	

public:

	AttackRange(IWorld* world, const GSvector3& positon, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	float lifeSpan_time;

};

#endif // !DAMAGE_RANGE_H_
