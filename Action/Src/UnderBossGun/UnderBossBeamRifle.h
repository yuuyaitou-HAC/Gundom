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
	UnderBossBeamRifle(IWorld* world, const GSvector3& position, const UnderBoss* underBoss);

	virtual void update(float delta_time)override;

	void fire();

private:
	void Cool(float delta_time);

private:

	const UnderBoss* underBoss_{ NULL };

	Player* player{ NULL };

private:

	int nowMagazine_{0};

	int assignmentMagazine_{0};

	float coolTimer_{120.0f};

 	const float assignmentCoolTimer_{120.0f};

	const float makeposOffset_{ 1.5f };

	const int attackValur_{ 5 };

	bool coolTimerTrigger_{false};
};
#endif // !BOSS_BEAMRIFLE_H_