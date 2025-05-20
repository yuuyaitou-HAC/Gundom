#ifndef UNDER_BOSS_BASTER_RIFLE_BULLET_H_
#define UNDER_BOSS_BASTER_RIFLE_BULLET_H_

#include "Collision/BasicAttackCollider.h"

class UnderBossBasterRiflrBullet :public BasicAttackCollider {

public:

	//コンストラクタ
	UnderBossBasterRiflrBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	//寿命
	float lifespanTimer_;

	GSquaternion quatenion_;

	GSuint bulletEffect_;

	GSmatrix4 effectSize_;
	GSvector3 bulletEffectScale_{ 2,2,2 };
};
#endif // !BASTER_RIFLE_BULLET_H_