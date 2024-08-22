#ifndef TANK_H_
#define TANK_H_

#include "Actor/Actor.h"
#include "Actor/ActorManager.h"

class Tank :public Actor {

public:
	enum calss State {

	};


public:

	Tank(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

private:




};

#endif // !TANK_H_
