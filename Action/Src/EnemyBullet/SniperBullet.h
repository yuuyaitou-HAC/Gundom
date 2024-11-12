#ifndef SNIPER_BULLET_H_
#define SNIPER_BULLET_H_

#include "Collision/BasicAttackCollider.h"

class SniperBullet : public BasicAttackCollider {

public:

	SniperBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

private:

	float lifespan_timer;

};

#endif // !SNIPER_BULLET_H_
