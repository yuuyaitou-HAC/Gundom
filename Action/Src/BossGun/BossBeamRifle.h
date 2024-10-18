#ifndef BOSS_BEAMRIFLE_H_
#define BOSS_BEAMRIFLE_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include <gslib.h>

class Boss;

class Player;

class BossBeamRifle :public Actor {

public:

	BossBeamRifle(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	void Fire();

private:

	Boss* boss;

	Player* player;

private:

	int NowMagazine;

	int AsignmentMagazine;

	float CoolTimer;

	float AsignmentCoolTimer;

	bool CoolTimerTrigger;

	float delta_time;

	void Cool();

	//アップデートで一回だけ呼ぶ
	bool a;

};


#endif // !BOSS_BEAMRIFLE_H_
