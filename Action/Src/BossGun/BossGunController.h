#ifndef BOSS_GUNCONTROLLER_H_
#define BOSS_GUNCONTROLLER_H_

#include "Actor/Actor.h"
#include <gslib.h>
#include "BossBeamRifle.h"
#include "Gatling.h"

class BossGunController : public Actor {
public:

	BossGunController(IWorld* world, const GSvector3& position);
	~BossGunController();

	virtual void update(float delta_time)override;

	virtual void changeState();

	void SetState(int num);

	void Fire();

private:

	BossBeamRifle* BR;
	Gatling* G;
	Boss* boss;

private:
	int GunNum;

};


#endif // !BOSS_GUNCONTROLLER_H_
