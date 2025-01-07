#ifndef TANK_AI_H_
#define TANK_AI_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Enemy/Tank.h"
#include "Collision/CollisionDetection.h"

class Player;

class EnemyShip;

class TankAI :public Actor {

private:

	std::vector<Tank*> tanks_;

public:

	TankAI(IWorld* world, const GSvector3& position);

	~TankAI();

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	//死亡した隊員が一定数超えたかどうか
	bool dieTrigger();

	//関数
private:

	//戦車生成
	void MakeTank();

	//目標地点が一定の範囲内かどうか
	bool PTRange(GSvector3 pos);

	//死んでいる数
	void DieCheack(float timer);

	//管轄下にある戦車が移動しているか判定
	bool MoveTrigger();

	//戦車の移動開始
	void MovePoint();

	void Updatepoint();


	//部隊の中心座標を決める関数
	GSvector3 centerOfCircle();


	//基本の座標
	void DesignatedPoint();

	//目標地点
	virtual GSvector3 AttackPoint();

	//参照
private:

	Player* player;

	EnemyShip* enemyship;

	CollisionDerection* cd_;

	//変数
private:

	//死亡した戦車の数
	int DieCounter;

	//死亡判定時間
	float DieTimer;

	bool Die;

	GSvector3 TargetPoint;

	//移動判定時間
	float MoveTimer;

	//プレイヤーと敵間の最小距離
//計算結果
	double PlayerToTank;

	//生成場所
	GSvector3 makepos;

	//目標地点の座標
	mutable GSvector3 Playerpos;

	//タンクの座標取得
	GSvector3 TanksPos;

	int MinDistance;

	int MaxDistance;

	float far = 0;

	float close = 1000;

	//目標地点とプレイヤーの座標を比較する間隔
	float pointtimer = 60.0f;
	float asignmentpointtimer = 60.0f;;

	GSvector3 result_;

	GSvector3 attackPoint_;

	bool AttackPointFrag_;

	int attackpointcounter;

	//部隊の中心座標
	GSvector3 center;

	//当たり判定の円の大きさ
	float radius = 5.0f;

	//ステージ上にある当たり判定をすべて格納する
	std::vector<Actor*> cds_;

};

#endif // !TANK_AI_H_
