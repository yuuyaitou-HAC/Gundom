#ifndef ATTACK_RANGE_H_
#define ATTACK_RANGE_H_

#include "Collision/BasicAttackCollider.h"

class Player;

class AttackRange :public BasicAttackCollider {

	

public:

	AttackRange(IWorld* world, const GSvector3& positon, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	float lifeSpan_time;

	Player* player;

	GSvector3 pos;

	//プレイヤーとの距離(前後)
	float Distance{ 1.5f };

	//高さ調整
	const float Hight{ 1.0f };

};

#endif // !DAMAGE_RANGE_H_
