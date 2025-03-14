#ifndef UNDER_BOSS_BEAMRIFLE_BULLET_H_
#define UNDER_BOSS_BEAMRIFLE_BULLET_H_

#include "Collision/BasicAttackCollider.h"

class UnderBossBeamRifleBullet : public BasicAttackCollider {

public:

	//コンストラクタ
	UnderBossBeamRifleBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	//寿命
	float lifespan_timer_;

	GSquaternion quatenion_;

	GSuint effect_handle_;
};
#endif // !BOSS_BEAMRIFLE_BULLET_H_