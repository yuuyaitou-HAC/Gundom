#ifndef TANK_AI_H_
#define TANK_AI_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Enemy/Tank.h"


class Player;

class TankAI :public Actor {

private:

	std::vector<Tank*> tanks_;

public:

	TankAI(IWorld* world, const GSvector3& position);

	~TankAI();

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other) override;

	//目標地点
	virtual GSvector3 AttackPoint() const;

	//関数
private:
	void MakeTank();

	//参照
private:

	Player* player;

	//変数
private:

	//生成場所
	GSvector3 makepos;

	//目標地点の座標
	mutable GSvector3 Playerpos;

	//呼び出された回数
	mutable int counter;

	//間の調整用の変数
	mutable float distance;

	//時間
	float MoveTimer;

	//プレイヤーと敵間の最小距離
	//計算結果
	double PlayerToTank;

	//保持しておく変数
	double PTT;

	//タンクの座標取得
	GSvector3 TanksPos;

	//戦車が移動中かどうか
	bool MoveTrigger;

	//目標地点が一定の範囲内かどうか
	bool PTRange(GSvector3 pos) const;

	//時間
	float DieTimer;

	int DieCounter;

	void DieCheack(float timer);

	bool trigger;



};

#endif // !TANK_AI_H_
