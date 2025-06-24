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
	float randPos_ = 5;

	//ボスのｙ軸
	float bossY_;

	//rayの長さ
	float rayLength_{ 30.0f };

	//攻撃力の倍率
	float magnificationAtttavkValue_{ 4 };

	//高さ
	float hight_{ 30 };

	//移動速度
	float speeed_{ 1.5f };

	//目標地点
	GSvector3 targetPoint_;

	//進行方向
	GSvector3 nowTargetPoint_;

	//着弾目標エフェクトの色
	GScolor targetPointEffectColor_ = { 1.0f,0.0f,0.0f,1.0f };

	//着弾目標エフェクトの大きさ
	GSvector3 targetPointEffectScall_ = { 2.0f,2.0f,2.0f };

	GSmatrix4 effectWorld_;

	AnimationMesh mesh_;

	Player* player_;

	Boss* boss_;

	//ミサイルとボスの距離
	float distanceY_;
};
#endif // !MISSILE_H_