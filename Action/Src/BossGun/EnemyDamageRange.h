#ifndef ENEMY_DAMAGE_RANGE_H_
#define ENEMY_DAMAGE_RANGE_H_

#include "Collision/BasicAttackCollider.h"

class EnemyDamageRange : public BasicAttackCollider {

public:

	//コンストラクタ
	EnemyDamageRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//衝突判定
	virtual void react(Actor& other)override;
private:

	//エフェクトの大きさ
	GSvector3 scall = { 2.0f,2.0f,2.0f };

	//エフェクトのハンドル
	GSuint effect_handle_;
};
#endif // !ENEMY_DAMAGE_RANGE_H_