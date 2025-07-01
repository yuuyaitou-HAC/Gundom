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

	AnimationMesh mesh_;

	Player* player_{ NULL };

	Boss* boss_{ NULL };

	//攻撃力の倍率
	const int magnificationAtttavkValue_{ 4 };

	int attackValur_{ 120 };

	//ミサイルとボスの距離
	float distanceY_{ 0.0f };

	//目標地点のランダム
	const float randPos_{ 5.0f };

	//ボスのｙ軸
	float bossY_{ 0.0f };

	//rayの長さ
	const float rayLength_{ 30.0f };

	//高さ
	const float hight_{ 30 };

	//移動速度
	const float speeed_{ 1.5f };

	//着弾地点のエフェクト
	GSuint targetPointEffect_{ 0 };

	//バーニアエフェクト
	GSuint vernierEffect_{ 0 };

	//爆発したか
	bool explosion_{ false };

	//一定値上昇したかどうか
	bool upFrag_{ false };

	//目標地点
	GSvector3 targetPoint_{ GSvector3().zero() };

	//進行方向
	GSvector3 nowTargetPoint_{ GSvector3().zero() };

	//着弾目標エフェクトの色
	const GScolor targetPointEffectColor_{ 1.0f,0.0f,0.0f,1.0f };

	//着弾目標エフェクトの大きさ
	const GSvector3 targetPointEffectScall_{ 2.0f,2.0f,2.0f };

	GSmatrix4 effectWorld_;
};
#endif // !MISSILE_H_