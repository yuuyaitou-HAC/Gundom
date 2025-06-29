#ifndef BEAM_MAGNUM_BULLET_H_
#define BEAM_MAGNUM_BULLET_H_

#include "Collision/BasicAttackCollider.h"

class BeamMagnumBullet :public BasicAttackCollider {

public:
	BeamMagnumBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	virtual void update(float delta_time)override;

private:

	float lifeSpanTime_{ 60.f };

	//エフェクトハンドル
	GSint bulletEffect_;

	GSquaternion quatenion_;
};
#endif // !BEAM_MAGNUM_H_