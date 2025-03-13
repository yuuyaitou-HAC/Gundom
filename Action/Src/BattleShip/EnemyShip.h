#ifndef ENEMY_SHIP_H_
#define ENEMY_SHIP_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "UnderBoss/UnderBoss.h"
#include "EnemyAI/TankAI.h"
#include "EnemyAI/HBMAI.h"
#include "Common/GameData.h"
#include "EnemyAI/EnemyBulletControl.h"


class EnemyShip : public Actor {
public:

	//コンストラクタ
	EnemyShip(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

private:

	UnderBoss* boss_;

	AnimationMesh mesh_;

	Player* player_;

	GSuint motion_;

	EnemyBulletControl* ebcontrol_;

	std::vector<TankAI*> tankais_;

	std::vector<HBMAI*> hbmais_;

private:

	void makeAI(float delta_time);

	//戦車生成
	void makeTankAI();

	//HBM生成
	void makeHbmAI(int weapon);

	//配列内で死んでいるものを調べる
	void diecheck();

private:

	//各個体の現在の生成数
	int nowTank = 0;
	int nowGatling = 0;
	int nowBeamRifle = 0;
	int nowBeamSaber = 0;
	int nowSniper = 0;

	//敵生成間隔
	float makeTimer_;

	float assignmentMakeTimer_ = 180.0f;

	//モーションループ
	bool motion_Loop_;

	//ボス登場フラグ
	bool bossFrag_;

	//ボスは生成したか
	bool bossMake_;

	//生成座標
	GSvector3 spawnPoint_;

	//自身の座標
	GSvector3 myPos_;
};
#endif // !ENEMY_SHIP_H_
