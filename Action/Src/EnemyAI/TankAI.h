#ifndef TANK_AI_H_
#define TANK_AI_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

class Player;

class Tank;

class TankAi : public Actor {

public:
	TankAi(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

	//目標地点
	virtual float Position();

private:

	virtual void MakeTank();

private:

	Actor tank;

	//生成個数
	int makeNumber;

	//プレイヤー
	Player* player;

	//戦車
	//Tank* tank;

	//プレイヤーの座標
	GSvector3 Playerpos;
	
	//自身の座標
	GSvector3 pos;

	//目標地点
	GSvector3 Destination;

	bool makeflag = false;

};


#endif // !TANK_AI_H_
