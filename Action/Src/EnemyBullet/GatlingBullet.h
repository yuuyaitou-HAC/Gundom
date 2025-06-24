#ifndef GATLING_BULLET_H_
#define GATLING_BULLET_H_

#include "Collision/BasicAttackCollider.h"
#include "Actor/AnimationMesh.h"

class GatlingBullet : public BasicAttackCollider {

public:

	//コンストラクタ
	GatlingBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//衝突リアクション
	virtual void react(Actor& other)override;


private:

	//寿命
	float lifespanTimer_{180.0f};

	GSquaternion quatenion_;

	GSuint effectHandle_;

	AnimationMesh mesh_;
};
#endif // !GATLING_BULLET_H_