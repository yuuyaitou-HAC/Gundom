#ifndef UNDER_BOSS_ATTACKRANGE_H_
#define UNDER_BOSS_ATTACKRANGE_H_

#include "Collision/BasicAttackCollider.h"

class UnderBossAttackRange :public BasicAttackCollider {

public:

	//コンストラクタ
	UnderBossAttackRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw() const override;

	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	float lifespan;

};

#endif // !BOSS_ATTACKRANGE_H_
