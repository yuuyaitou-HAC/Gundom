#ifndef DAMAGE_RANGE_H_
#define DAMAGE_RANGE_H_

#include "Collision/BasicAttackCollider.h"

class DamageRange :public BasicAttackCollider {

public:

	DamageRange(IWorld* world, const GSvector3& positon, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	virtual void draw() const override;

	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	GSuint effect_handle;
};
#endif // !DAMAGE_RANGE_H_