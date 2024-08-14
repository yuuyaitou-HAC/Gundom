#ifndef BEAM_MAGNUM_BULLET_H_
#define BEAM_MAGNUM_BULLET_H_

#include "Collision/BasicAttackCollider.h"

class BeamMagnumBullet :public BasicAttackCollider {

public:
	BeamMagnumBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	virtual void update(float delta_time)override;

	virtual void draw() const override;

	virtual void react(Actor& other)override;

private:

	float lifeSpan_time_;


};

#endif // !BEAM_MAGNUM_H_
