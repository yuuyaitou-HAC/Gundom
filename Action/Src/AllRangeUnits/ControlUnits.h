#ifndef CONTROL_UNITS_H_
#define CONTROL_UNITS_H_

#include "Actor/Actor.h"
#include "AllRangeUnit.h"
#include "Collision/CollisionDetection.h"

class Player;

class ControlUnits : public Actor {

private:
	std::vector<AllRangeUnit*> units_;

public:

	ControlUnits(IWorld* world, const GSvector3& position);

	~ControlUnits();

	virtual void update(float delta_time)override;

	virtual void draw() const override;

	virtual void react(Actor& other)override;

	void changeFrag(bool frag);

private:

	void makeUnits();

	void settarget();

	void retreat();

	bool StateNow(AllRangeUnit::State state);

	void Enemyarraymanagement();

	void diechack();

	Actor* PickTarget();

private:

	Player* player_;

	CollisionDerection* cd_;

	std::vector<Actor*> enemys_;

private:

	GSvector3 pos;

	//çUåÇÇ©ìPëﬁÇ©
	bool Change;

	bool MakeFrag = false;

	int sarchcounter;

	int diecounter = 0;

	int test;

};

#endif // !CONTROL_UNITS_H_
