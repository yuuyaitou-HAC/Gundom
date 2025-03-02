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
	GSint effect_handle;

	//寿命
	float lifespan_timer_;

	//爆発したかの判定
	bool explosion = false;

	GSmatrix4 local_matrix;

	AnimationMesh mesh_;

	Player* player_;
};
#endif