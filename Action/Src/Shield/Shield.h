#ifndef SHIELD_H_
#define SHIELD_H_

#include "Actor/Actor.h"

class Shield :public Actor {

public:

	Shield(IWorld* world, const GSvector3 pos);

	void update(float delta_time);

	void draw() const override;
	virtual void react(Actor& other)override;

};

#endif // !SHIELD_H_
