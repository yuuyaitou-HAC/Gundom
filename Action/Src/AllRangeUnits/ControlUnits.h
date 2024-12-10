#ifndef CONTROL_UNITS_H_
#define CONTROL_UNITS_H_

#include "Actor/Actor.h"
#include "AllRangeUnit.h"

class Player;

class ControlUnits : public Actor {

private:
	std::vector<AllRangeUnit*> units_;

public:

	ControlUnits(IWorld* world, const GSvector3& position);

	~ControlUnits();

	virtual void update(float delta_time)override;

	void changeFrag(bool frag);

private:

	void makeUnits();

	void retreat();

private:

	Player* player_;

private:

	bool Change;

};

#endif // !CONTROL_UNITS_H_
