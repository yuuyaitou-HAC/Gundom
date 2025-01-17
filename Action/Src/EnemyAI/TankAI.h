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

	bool retreatFrag();

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

	void retreat();


	//参照
private:

	Player* player;

	EnemyShip* enemyship;

	CollisionDerection* cd_;

	//変数
private:

	//死亡した戦車の数
	int DieCounter;

	//プレイヤーに接近できる距離
	int MinDistance;
	int MaxDistance;

	int attackpointcounter;

	int DesignatedPointcounter;

	float far = 0;

	float close = 1000;

	//死亡判定時間
	float DieTimer;

	//移動判定時間
	float MoveTimer;

	//目標地点とプレイヤーの座標を比較する間隔
	float pointtimer = 60.0f;
	float asignmentpointtimer = 60.0f;

	//当たり判定の円の大きさ
	float radius = 5.0f;

	//プレイヤーと敵間の最小距離
	double PlayerToTank;

	//自身の死亡
	bool DieAI;

	//部隊の死亡状況
	bool Die;

	//目標地点設定できたか
	bool AttackPointFrag_;

	//撤退中か？
	bool noposition;

	//座標更新中か
	bool updatepoint;

	GSvector3 TargetPoint;

	//生成場所
	GSvector3 makepos;

	//目標地点の座標
	GSvector3 Playerpos;

	//タンクの座標取得
	GSvector3 TanksPos;

	GSvector3 result_;

	GSvector3 attackPoint_;

	//部隊の中心座標
	GSvector3 center;

	//プレイヤー座標のy軸を抜いたやつ
	GSvector3 playerposxz;

	//ステージ上にある当たり判定をすべて格納する
	std::vector<Actor*> cds_;
};

#endif // !TANK_AI_H_
