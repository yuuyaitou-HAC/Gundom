#ifndef GATLING_H_
#define GATLING_H_

#include "Actor/Actor.h"
#include <gslib.h>

class Player;
class UnderBoss;

class Gatling : public Actor {

public:

	Gatling(IWorld* world, const GSvector3& position, const UnderBoss* underBoss);

	virtual void update(float delta_time)override;

	void Fire();

private:
	void Cool(float delta_time);

private:

	const UnderBoss* underBoss_{ NULL };

	Player* player_{ NULL };

private:

	//現在のマガジン数
	int nowMagazine_{ 0 };

	int assignmentMagazine_{ 0 };

	int attackValue_{ 40 };

	float coolTimer_{ 240.0f };

	const float assignmentCoolTimer_{ 240.0f };

	//クールタイム中か
	bool coolTimerTrigger_{ false };

	//ガトリングの拡散
	GSvector2 randam_{ -2,2 };
};
#endif // !GATLING_H_