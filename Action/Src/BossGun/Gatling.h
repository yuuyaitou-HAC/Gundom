#ifndef GATLING_H_
#define GATLING_H_

#include "Actor/Actor.h"
#include <gslib.h>

class Player;
class Boss;

class Gatling : public Actor {

public:

	Gatling(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

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

	float delta_timer;

	void Cool();


	GSvector2 randam;

	//アップデート時に一回だけ呼ばれる
	bool a;

};


#endif // !GATLING_H_
