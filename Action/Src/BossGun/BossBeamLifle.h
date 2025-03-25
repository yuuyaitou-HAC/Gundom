#ifndef BOSS_BEAM_LIFLE_H_
#define BOSS_BEAM_LIFLE_H_

#include "Collision/BasicAttackCollider.h"


class BossBeamLifle : public BasicAttackCollider {
public:

	BossBeamLifle(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	virtual void update(float delta_time)override;

	virtual void react(Actor& other)override;

private:

	//寿命
	float lifeSpanTimer_;

	//エフェクトハンドル
	GSuint effectHandle_;

	GSquaternion quatenion_;

};
#endif // !BOSS_BEAM_LIFLE_H_