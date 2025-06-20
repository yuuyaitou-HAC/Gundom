#ifndef ENEMY_ATTACK_RANGE_H_
#define ENEMY_ATTACK_RANGE_H_

#include "Collision/BasicAttackCollider.h"

class EnemyAttackRange : public BasicAttackCollider {
public:

	EnemyAttackRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	virtual void update(float delta_time)override;

	virtual void react(Actor& other)override;

private:

	//エフェクトのハンドル
	GSuint effectHandle_;

	//エフェクトの色(紫)
	GScolor4 slashEffectColor_{ 0.86f,0.298f,1.0f,1.0f };
};
#endif // !ENEMY_ATTACK_RANGE_H_