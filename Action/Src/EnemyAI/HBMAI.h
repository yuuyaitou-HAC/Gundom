#ifndef HBMAI_H_
#define HBMAI_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Enemy/HBM.h"

class Player;

class EnemyShip;

class HBMAI : public Actor {

private:

	std::vector<HBM*> hbms_;

public:

	HBMAI(IWorld* world, const GSvector3& position);

	~HBMAI();

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	//目標地点
	virtual GSvector3 AttackPoint() const;

	//死亡した隊員が一定数超えたかどうか
	bool dieTrigger();

private:

	//戦車生成
	void MakeHBM();

	//目標地点が一定の範囲内かどうか
	bool PTRange(GSvector3 pos) const;

	//死んでいる数
	void DieCheack(float timer);

	//管轄下にある戦車が移動しているか判定
	bool MoveTrigger();

	//戦車の移動開始
	void MovePoint();

	//自身の座標をプレイヤーから一番離れている戦車の座標に変える変数
	void search();

	//参照
private:

	Player* player;

	EnemyShip* enemyship;

	//変数
private:

	//死亡した戦車の数
	int DieCounter;

	//武器の種類
	int weapon;

	//死亡判定時間
	float DieTimer;

	bool Die;

	//移動判定時間
	float MoveTimer;

	//プレイヤーと敵間の最小距離
//計算結果
	double PlayerToHBM;

	//生成場所
	GSvector3 makepos;

	//目標地点の座標
	mutable GSvector3 Playerpos;

	//タンクの座標取得
	GSvector3 HBMPos;

	//プレイヤーとの距離
	float MinDistance;
	float MaxDistance;

	//装備している武器に応じてプレイヤーの視界から広がれる角度
	float weaponangle;

	//プレイヤーとの距離が一番遠い
	float far = 0;

	//プレイヤーとの距離が一番近い
	float close = 1000;
};


#endif // !HBMAI_H_