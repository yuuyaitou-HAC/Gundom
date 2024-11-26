#ifndef MISSION_H_
#define MISSION_H_

#include "Actor/Actor.h"

#include "Player/Player.h"
#include "BOSS/Boss.h"
#include "Common/GameData.h"

class Mission : public Actor {

	enum class State {
		Mission1,
		Mission2,
		GameClear
	};

public:

	Mission(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time) override;

	virtual void draw() const override;

private:

	void Mission1(float delta_time);

	void Mission2(float delta_time);

	void GameClear(float delta_time);

private:

	Player* player_;

	Boss* boss_;

	State state_;

private:

	float MissionTimer;

};

#endif // !MISSION_H_
