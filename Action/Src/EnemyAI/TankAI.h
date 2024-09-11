#ifndef TANK_AI_H_
#define TANK_AI_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

class Player;

class TankAI :public Actor {

public:

	TankAI(IWorld* world,const GSvector3& position);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other) override;

	//ŠÖ”
private:
	void MakeTank();

	//QÆ
private:

	Player* player;

	//•Ï”
private:

	//íÔ‚Ì¶¬ŒÂ”
	int MakeNumber;

	GSvector3 makepos;

};

#endif // !TANK_AI_H_
