#ifndef ENEMY_SHIP_H_
#define ENEMY_SHIP_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "BOSS/Boss.h"
#include "EnemyAI/TankAI.h"
#include "EnemyAI/HBMAI.h"

class EnemyShip : public Actor {
public:

	EnemyShip(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

private:

	Boss* boss_;

	AnimationMesh mesh_;

	GSuint motion_;

	bool motion_loop_;

	std::vector<TankAI*> tankais_;

	std::vector<HBMAI*> hbmais_;

private:


	//敵生成
	void makeTankAI();

	void makeHbmAi();

private:

	//生成座標
	GSvector3 Spawnpoint;


	//敵生成間隔
	float MakeTimer;

	//自身の生成した敵の数
	int makeCounter;

	//生成数上限
	int MaximumNumberGenerated;

	//生成時間ランダム
	GSvector2 MakeTimerRand{ 300.0f,600.0f };

	//死んだ戦車部隊の数
	int DieTankNum;

	//ボス登場フラグ
	bool BossFrag;

	//死んだ数
	int diecounter;

	bool bossmake;

	GSvector3 pos;

	//ランダムで生成するものを決める
	GSvector2 makerand{ 1,2 };

	//配列内で死んでいるものを調べる
	void  diecheck();

};


#endif // !ENEMY_SHIP_H_
