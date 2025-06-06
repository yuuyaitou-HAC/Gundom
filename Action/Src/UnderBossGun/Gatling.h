#ifndef GATLING_H_
#define GATLING_H_

#include "Actor/Actor.h"
#include <gslib.h>

class Player;
class UnderBoss;

class Gatling : public Actor {

public:

	Gatling(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	void Fire();

private:
	void Cool();

private:

	UnderBoss* boss_;

	Player* player_;

private:

	int nowMagazine_;

	int assignmentMagazine_;

	float coolTimer_;

	float assignmentCoolTimer_;

	bool coolTimerTrigger_;

	float deltaTimer_;

	GSvector2 randam_;

	//アップデート時に一回だけ呼ばれる
	bool oneTrigger_;
};
#endif // !GATLING_H_