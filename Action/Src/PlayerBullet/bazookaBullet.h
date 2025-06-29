#ifndef BAZOOKA_BULLET_H_
#define BAZOOKA_BULLET_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Collision/BasicAttackCollider.h"
#include "Player/Player.h"

//プレイヤーの弾クラス
class BazookaBullet :public BasicAttackCollider {
public:
	//コンストラクタ
	BazookaBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//衝突リアクション
	virtual void react(Actor& other)override;

private:

	//エフェクトハンドル
	GSint vernierEffect_;

	const GSvector3 vernierEffectPos_{ 0.0f,0.0f,-1.5f };
	const GSvector3 vernierEffectEuler_{ GSvector3::zero() };
	const GSvector3 vernierEffectScale_{ 1,1,1 };

	//寿命
	float lifespanTimer_{ 60.0f };

	//爆発したかの判定
	bool explosion_{ false };

	GSmatrix4 localMatrix_;
	GSmatrix4 effectWorld_;

	AnimationMesh mesh_;

	Player* player_{ NULL };
};
#endif