#pragma once

#ifndef PLAYER_BULLET_H_
#define PLAYER_BULLET_H_

//#include "Actor/Actor.h"

#include "Collision/BasicAttackCollider.h"
#include "Player/PlayerState.h"


//プレイヤーの弾クラス
class PlayerBullet :public BasicAttackCollider {
public:
	//コンストラクタ
	PlayerBullet(IWorld* world, const GSvector3& positon, const GSvector3& velocity,int Damage,std::string name);
	
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//衝突リアクション
	virtual void react(Actor& other)override;

private:
	//寿命
	float lifespan_timer_;

	PlayerState* playerstate_;

};

#endif

