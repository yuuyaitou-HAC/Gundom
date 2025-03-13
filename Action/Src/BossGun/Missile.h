#ifndef MISSILE_H_
#define MISSILE_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Collision/BasicAttackCollider.h"
#include "Player/Player.h"
#include "Boss/Boss.h"

class Missile : public BasicAttackCollider {

public:
	Missile(IWorld* world, const GSvector3& position, const GSvector3& velocity, int damage);

	~Missile();

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

private:

	//着弾地点のエフェクト
	GSuint targetPointEffect_;

	//バーニアエフェクト
	GSuint vernierEffect_;

	bool explosion_ = false;

	//一定値上昇したかどうか
	bool upFrag_;

	//目標地点のランダム
	float randpos_ = 5;

	//ボスのｙ軸
	float bossY_;

	//目標地点
	GSvector3 targetPoint_;

	//進行方向
	GSvector3 nowTargetPoint_;

	GScolor color_ = { 1.0f,0.0f,0.0f,1.0f };
	GSvector3 scall_ = { 2.0f,2.0f,2.0f };

	GSmatrix4 effectWorld_;

	AnimationMesh mesh_;

	Player* player_;

	Boss* boss_;
};
#endif // !MISSILE_H_