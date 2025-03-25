#ifndef ENEMY_ATTACK_CONTROL_H_
#define ENEMY_ATTACK_CONTROL_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "EnemyAI/HBMAI.h"
#include "EnemyAI/TankAI.h"

class EnemyAttackControl :public Actor {

public:

	//コンストラクタ
	EnemyAttackControl(IWorld* world, const GSvector3& position);

	//デストラクタ
	~EnemyAttackControl();

	//更新
	virtual void update(float delta_time)override;

	//それぞれの部隊を配列に入れるための関数
	void setBeamLifleAI(HBMAI* AI);
	void setGatlingAI(HBMAI* AI);
	void setTanckAI(TankAI* AI);

private:

	//死んでいるもしくは撤退中の固体を配列から排除
	void sarch();

	//攻撃命令を出す
	void attackBeamLifle(float delta_time);
	//void attackGatling(float delta_time);
	//void attackTanck(float delta_time);

	////攻撃命令を出す mission3用
	//void attackBeamLifleMission3(float delta_time);
	//void attackGatlingMission3(float delta_time);
	//void attackTanckMission3(float delta_time);

private:

	//各AIを格納する配列
	std::vector<HBMAI*> beamLifleAIs_;
	std::vector<HBMAI*> gatlingAIs_;
	std::vector<TankAI*>tankAIs_;

	//呼び出すAIを入れる変数
	HBMAI* hbmai1_;
	HBMAI* hbmai2_;
	HBMAI* hbmai3_;

	TankAI* tankai1_;
	TankAI* tankai2_;
	TankAI* tankai3_;

	////呼び出す番号
	//int beamLifleAICallNumber_ = 0;
	//


	//int gatringAICallNumber_ = 0;
	//int tankAICallNumber_ = 0;

	////弾切れ起こした個体をカウント
	//int tankCounter_;

	////クールタイム
	//float gatringAttackTime_ = 180.0f;
	//float tankAttackTime_ = 120.0f;

	////NULLの時の時間
	//float hbmprocessingTimer3_ = 300.0f;

	////NULLの時の時間
	//float tankprocessingTimer1_ = 300.0f;
	//float tankprocessingTimer2_ = 300.0f;
	//float tankprocessingTimer3_ = 300.0f;


	////呼び出した個体の状態に応じて処理を変える
	//bool beamLifleCall3_;

	//bool tankCall1_;
	//bool tankCall2_;
	//bool tankCall3_;

	//再構築後の変数

	//呼び出す個体の番号
	int beamLifleAICallNumber1_;
	int beamLifleAICallNumber2_;

	//次の呼び出し処理までの時間
	float beamLifleAttackTime_ = 180.0f;


	//割り当てられた個体がNULLかどうか
	bool beamLifleNULL1_;
	bool beamLifleNULL2_;
	
	//射撃の処理が終了したか
	bool beamLifleComple1_;
	bool beamLifleComple2_;

	//呼び出し個体の割り当て終了したか
	bool beamLifleCallComple_ = false;


};
#endif // !ENEMY_BULLET_CONTROL_H_