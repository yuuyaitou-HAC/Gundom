#ifndef PLAYER_AHIP_H_
#define PLAYER_AHIP_H_

#include "Actor/Actor.h"
#include "Common/GameData.h"
#include "Collision/CollisionDetection.h"
#include "string"
#include "Player/Player.h"

class PlayerShip : public Actor {
public:

	//コンストラクタ
	PlayerShip(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//衝突判定
	virtual void react(Actor& other)override;

private:

	//補給
	void supply();

	void delay(float delta_time);

private:

	AnimationMesh mesh_;

	CollisionDerection* cd_;

	Player* player_;

private:

	//自身の座標
	GSvector3 pos;

	GSvector3 playerPos_;

	//補給後から出撃までの処理
	bool delayFrag_;

	//補給後から出撃までの時間
	float delayTimer_ = 300.0f;
	float assignmentDelayTimer_ = 300.0f;
};

#endif // !PLAYER_AHIP_H_
