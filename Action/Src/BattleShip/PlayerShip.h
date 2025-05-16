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

private:

	void move(float delta_time);

	//補給
	void supply();

	void delay(float delta_time);

private:

	AnimationMesh mesh_;

	CollisionDerection* cd_;

	Player* player_;

private:

	//補給後から出撃までの時間
	float delayTimer_ = 300.0f;
	float assignmentDelayTimer_ = 300.0f;

	//補給後から出撃までの処理
	bool delayFrag_;

	GSvector3 playerPos_;


	float timeElapsed_ = 0.0f;
	GSvector3 basePosition_;

	float amplitude = 0.05f;
	float frequency = 0.005f;
};

#endif // !PLAYER_AHIP_H_
