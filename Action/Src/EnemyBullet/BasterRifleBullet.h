#ifndef BASTER_RIFLE_BULLET_H_
#define BASTER_RIFLE_BULLET_H_

#include "Collision/BasicAttackCollider.h"

class BasterRiflrBullet :public BasicAttackCollider {

public:

	//コンストラクタ
	BasterRiflrBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	//寿命
	float lifespan_timer_;

	GSquaternion quatenion;

	GSuint effect_handle;

};


#endif // !BASTER_RIFLE_BULLET_H_
