#ifndef UNDER_BOSS_BEAMRIFLE_H_
#define UNDER_BOSS_BEAMRIFLE_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include <gslib.h>

class UnderBoss;

class Player;

class UnderBossBeamRifle :public Actor {

public:

	//コンストラクタ
	UnderBossBeamRifle(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	void fire();

private:
	void Cool(float delta_time);

private:

	UnderBoss* boss;

	Player* player;


private:

	int nowMagazine_;

	int assignmentMagazine_;

	float coolTimer_;

	float assignmentCoolTimer_;

	bool coolTimerTrigger_;

	bool oneTrigger_;
};
#endif // !BOSS_BEAMRIFLE_H_