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

	////補給
	//void supply();

	//void delay(float delta_time);

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

	GSvector3 myPos_;


	float timeElapsed_ = 0.0f;
	GSvector3 basePosition_;

	float amplitude_ = 0.05f;
	float frequency_ = 0.005f;

	GSuint vernier1_;
	GSuint vernier2_;
	GSuint vernier3_;

	//地面の砂埃
	GSuint dustEffect_;

	GSvector3 vernierEffectPos1_{ -23,-2,0 };
	GSvector3 vernierEffectPos2_{ 10,-4,-5 };
	GSvector3 vernierEffectPos3_{ 10,-4,5 };

	GSvector3 scal_;

	GSvector3 playerPos_;
	GSvector3 effectPos_;

	bool effectTrigger_;
};
#endif // !PLAYER_AHIP_H_