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

	//目標地点
	virtual GSvector3 AttackPoint() const;

	//関数
private:

	//戦車生成
	void MakeTank();

	//目標地点が一定の範囲内かどうか
	bool PTRange(GSvector3 pos) const;

	//死んでいる数
	void DieCheack(float timer);

	//管轄下にある戦車が移動しているか判定
	bool MoveTrigger();

	//戦車の移動開始
	void MovePoint();

	//参照
private:

	Player* player;



	//変数
private:

	//死亡した戦車の数
	int DieCounter;

	//死亡判定時間
	float DieTimer;

	//移動判定時間
	float MoveTimer;

	//プレイヤーと敵間の最小距離
//計算結果
	double PlayerToTank;

	//保持しておく変数
	double PTT;

	//生成場所
	GSvector3 makepos;

	//目標地点の座標
	mutable GSvector3 Playerpos;

	//タンクの座標取得
	GSvector3 TanksPos;
};

#endif // !TANK_AI_H_
