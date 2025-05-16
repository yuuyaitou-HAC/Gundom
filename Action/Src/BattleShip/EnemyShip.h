#ifndef ENEMY_SHIP_H_
#define ENEMY_SHIP_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "UnderBoss/UnderBoss.h"
#include "EnemyAI/TankAI.h"
#include "EnemyAI/HBMAI.h"
#include "Common/GameData.h"
#include "EnemyAI/EnemyAttackControl.h"


class EnemyShip : public Actor {
public:

	//コンストラクタ
	EnemyShip(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

private:

	void move(float delta_time);

	void makeAI(float delta_time);

	//ミッション1
	void mission1MakeAi();

	//ミッション2
	void mission2MakeAi();

	//ミッション3
	void mission3MakeAi();

	//ミッション4
	void mission4MakeAi();

	//戦車生成
	void makeTankAI();

	//HBM生成
	void makeHbmAI(int weapon);

	//ミッション２移行時に撤退命令
	void retreatmission2();

	//ミッション３移行時に撤退命令
	void retreatmission4();

	//配列内で死んでいるものを調べる
	void diecheck();

private:

	UnderBoss* boss_;

	AnimationMesh mesh_;

	Player* player_;

	GSuint motion_;

	EnemyAttackControl* ebcontrol_;

	std::vector<TankAI*> tankais_;

	std::vector<HBMAI*> hbmais_;

	//各武器ごとの配列
	std::vector<HBMAI*> beamSaber_;
	std::vector<HBMAI*> Gatring_;
	std::vector<HBMAI*> beamRifle_;
	std::vector<TankAI*> tank_;
private:

	//各個体の現在の生成数
	int nowTank_ = 0;
	int nowGatling_ = 0;
	int nowBeamRifle_ = 0;
	int nowBeamSaber_ = 0;
	int nowSniper_ = 0;

	//カウントした敵の数
	int tankCounter_ = 0;
	int beamRifleCounter_ = 0;
	int gatringCounter_ = 0;

	//敵生成間隔
	float makeTimer_;

	float assignmentMakeTimer_ = 180.0f;

	//撤退終了したか
	bool retreatMission2Frag_ = false;

	//撤退終了したか
	bool retreatMission4Frag_ = false;

	//排除中かどうか
	bool finishRetreatFrag = false;

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

	float timeElapsed_ = 0.0f;
	GSvector3 basePosition_;

	// 上下の移動幅（最大±amplitudeだけ上下する）
	float amplitude = 0.05f;        // 上下の高さ
	float frequency = 0.005f;        // 周期（1秒で1往復）

	GSuint vernier1;
	GSuint vernier2;
	GSuint vernier3;

	//地面の砂埃
	GSuint dust;

	GSvector3 test1{ -23,-2,0 };
	GSvector3 test2{ 10,-4,-5 };
	GSvector3 test3{ 10,-4,5 };

	GSvector3 scal;

	GSvector3 playerpos_;
	GSvector3 effectpos_;

	bool effectTrigger_;

};
#endif // !ENEMY_SHIP_H_