#ifndef BOSS_BEAMRIFLE_H_
#define BOSS_BEAMRIFLE_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include <gslib.h>

class Boss;

class Player;

class BossBeamRifle :public Actor {

public:

	//コンストラクタ
	BossBeamRifle(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;
	
	void fire();

private:

	Boss* boss;

	Player* player;

private:
	void Cool(float delta_time);

private:

	int NowMagazine_;

	int AsignmentMagazine_;

	float CoolTimer_;

	float AsignmentCoolTimer_;

	bool CoolTimerTrigger_;

	bool FarstUpdate_;
};


#endif // !BOSS_BEAMRIFLE_H_
