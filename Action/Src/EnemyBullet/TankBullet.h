#pragma once

#ifndef TANK_BULLET_H_
#define TANK_BULLET_H_

#include "Collision/BasicAttackCollider.h"
#include "Enemy/Tank.h"

class TankBullet : public BasicAttackCollider {

public:

	//コンストラクタ
	TankBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	//寿命
	float lifespan_timer_;

	Tank* tank_;

};


#endif // !TANK_BULLET_H_

