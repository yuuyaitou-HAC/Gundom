#pragma once

#ifndef TANK_BULLET_H_
#define TANK_BULLET_H_

#include "Collision/BasicAttackCollider.h"

class TankBullet : public BasicAttackCollider {

public:

	//コンストラクタ
	TankBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);
	~TankBullet();
	//更新
	virtual void update(float delta_time)override;

	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	//寿命
	float lifespanTimer_;

	GSquaternion quatenion_;

	GSuint bulletEffect_;

	GSvector3 bulletEffectScale_{ 2.0f,2.0f,2.0f };

	GSmatrix4 effectSize_;
};
#endif // !TANK_BULLET_H_