#ifndef MISSILE_DAMAGE_RANGE_H_
#define MISSILE_DAMAGE_RANGE_H_

#include "Collision/BasicAttackCollider.h"

class MissileDamageRange : public BasicAttackCollider {

public:

	//コンストラクタ
	MissileDamageRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	//衝突判定
	virtual void react(Actor& other)override;
private:

	//エフェクトの大きさ
	GSvector3 scall{ 2.0f,2.0f,2.0f };

	//エフェクトのハンドル
	GSuint effect_handle_{ 0 };
};
#endif // !ENEMY_DAMAGE_RANGE_H_