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

	void changeFrag(bool frag);

private:

	void makeUnits();

	void settarget();

	void retarget();

	void retreat();

	Actor* searchtaget();

	bool StateNow(AllRangeUnit::State state);

private:

	Player* player_;

	CollisionDerection* cd_;

	std::vector<Actor*> enemys_;

private:

	//‰‚ß‚Ìİ’è‚©‚Ç‚¤‚©
	bool SetFrag;

	//UŒ‚‚©“P‘Ş‚©
	bool Change;

	bool MakeFrag = false;

	Actor* enemy;

	//æ“¾‚µ‚½“G‚ª“¯‚¶‚©
	bool same;
};

#endif // !CONTROL_UNITS_H_
