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

	//目標地点
	virtual GSvector3 point() const;

	//関数
private:
	void MakeTank();

	//参照
private:

	Player* player;

	//変数
private:

	//戦車の生成個数
	int MakeNumber;
	
	//生成場所
	GSvector3 makepos;

	//目標地点の座標
	mutable GSvector3 Playerpos;

	//呼び出された回数
	mutable int counter;

	//間の調整用の変数
	float Adjustment;

};

#endif // !TANK_AI_H_
