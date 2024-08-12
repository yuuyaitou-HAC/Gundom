#ifndef BAZOOKA_H_
#define BAZOOKA_H_

#include "Actor/Actor.h"

#include "Collision/BasicAttackCollider.h"
#include "Player/Player.h"


//プレイヤーの弾クラス
class Bazooka :public BasicAttackCollider {
public:
	//コンストラクタ
	Bazooka(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage);

	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//衝突リアクション
	virtual void react(Actor& other)override;

private:
	//寿命
	float lifespan_timer_;

	Player* player_;

	//爆発したかの判定
	bool explosion = false;

};

#endif

