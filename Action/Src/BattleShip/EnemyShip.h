#ifndef ENEMY_SHIP_H_
#define ENEMY_SHIP_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "BOSS/Boss.h"
#include "EnemyAI/TankAI.h"
#include "EnemyAI/HBMAI.h"


class EnemyShip : public Actor {
public:

	//コンストラクタ
	EnemyShip(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//衝突判定
	virtual void react(Actor& other)override;

private:

	Boss* boss_;

	AnimationMesh mesh_;

	Player* player_;

	GSuint motion_;


	std::vector<TankAI*> tankais_;

	std::vector<HBMAI*> hbmais_;

private:

	//戦車生成
	void makeTankAI();

	//HBMの武器の確率
	int randWeapon();

	//HBM生成
	void makeHbmAi();

	//配列内で死んでいるものを調べる
	void diecheck();

private:

	//自身の生成した敵の数
	int MakeCounter_;

	//生成数上限
	int MaximumNumberGenerated_;

	//死んだ戦車部隊の数
	int DieTankNum_;

	//死んだ数
	int DieCounter_;

	//敵生成間隔
	float MakeTimer_;

	//モーションループ
	bool Motion_Loop_;

	//ボス登場フラグ
	bool BossFrag_;

	//ボスは生成したか
	bool BossMake_;

	//生成時間ランダム
	GSvector2 MakeTimerRand_{ 300.0f,600.0f };

	//ランダムで生成するものを決める
	GSvector2 MakeRand_{ 1,2 };

	//生成座標
	GSvector3 SpawnPoint_;

	//自身の座標
	GSvector3 MyPos_;

};


#endif // !ENEMY_SHIP_H_
