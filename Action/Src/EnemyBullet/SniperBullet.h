#ifndef SNIPER_BULLET_H_
#define SNIPER_BULLET_H_

#include "Collision/BasicAttackCollider.h"

class SniperBullet : public BasicAttackCollider {

public:

	SniperBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);
	~SniperBullet();
	virtual void update(float delta_time)override;

	virtual void react(Actor& other)override;

private:

	float lifespanTimer_{ 180.0f };

	GSquaternion quatenion_;

	GSuint bulletEffect_{ 0 };

	GSmatrix4 effectSize_;

	//エフェクトの大きさ
	const GSvector3 bulletEffectScale_{ 2.0f,2.0f,2.0f };
};
#endif // !SNIPER_BULLET_H_
