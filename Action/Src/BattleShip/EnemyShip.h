#ifndef ENEMY_SHIP_H_
#define ENEMY_SHIP_H_

#include "Actor/Actor.h"
#include "BOSS/Boss.h"
#include "EnemyAI/TankAI.h"

class EnemyShip : public Actor {
public:

	EnemyShip(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

private:

	Boss* boss_;
	TankAI* tankai_;

private:

	//“G¶¬
	void MakeEnemy(float delta_time);

	//“G¶¬ŠÔŠu
	float MakeTimer;

	//¶¬ŠÔƒ‰ƒ“ƒ_ƒ€
	GSvector2 MakeTimerRand{ 60.0f,120.0f };

	//€‚ñ‚¾íÔ•”‘à‚Ì”
	int DieTankNum;

	//ƒ{ƒX“oêƒtƒ‰ƒO
	bool BossFrag;

};


#endif // !ENEMY_SHIP_H_
