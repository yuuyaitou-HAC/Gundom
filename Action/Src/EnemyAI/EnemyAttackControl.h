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
	void set_beamRifleAI(HBMAI* AI);
	void set_gatlingAI(HBMAI* AI);
	void set_tankAI(TankAI* AI);

private:

	//死んでいるもしくは撤退中の固体を配列から排除
	void sarch();

	//攻撃命令を出す
	void attack_beamRifle(float delta_time);
	void attack_gatling(float delta_time);
	void attack_tank(float delta_time);

private:

	//各AIを格納する配列
	std::vector<HBMAI*> beamRifleAIs_;
	std::vector<HBMAI*> gatlingAIs_;
	std::vector<TankAI*>tankAIs_;

	//呼び出すAIを入れる変数
	HBMAI* beamRifleAI1_{ NULL };
	HBMAI* beamRifleAI2_{ NULL };
	HBMAI* GatlingAI_{ NULL };
	TankAI* tankai1_{ NULL };
	TankAI* tankai2_{ NULL };

	//呼び出す個体の番号
	int beamRifleAICallNumber1_{ 0 };
	int beamRifleAICallNumber2_{ 0 };

	//戦車
	int tankAICallNumber1_{ 0 };
	int tankAICallNumber2_{ 0 };

	//呼び出し番号
	int gatlingAICallNumber_{ 0 };

	//次の呼び出しまでの時間
	float gatlingAttackTime_{ 180.0f };
	float assignmentGatlingAttackTime_{ 180.0f };

	//次の呼び出し処理までの時間
	float beamRifleAttackTime_{ 180.0f };
	float assignmentBeamRifleAttackTimer_{ 180.0f };

	//次の呼び出し処理までの時間
	float tankAttackTime_{ 180.0f };
	float assignmenttankAttackTime_{ 180.0f };

	//割り当てられた個体がNULLかどうか
	bool beamRifleNULL1_{ false };
	bool beamRifleNULL2_{ false };

	//射撃の処理が終了したか
	bool beamRifleComple_{ false };
	bool beamRifleComple2_{ false };

	//呼び出し個体の割り当て終了したか
	bool beamRifleCallComple_{ false };

	//割り当てられた個体がNULLかどうか
	bool tankNULL1_{ false };
	bool tankNULL2_{ false };

	//射撃の処理が終了したか
	bool tankComple1_{ false };
	bool tankComple2_{ false };

	//呼び出し個体の割り当て終了したか
	bool tankCallComple_{ false };

	//呼び出した対象がNULLの場合
	bool gatlingNULL_{ false };

	//射撃終了したら
	bool gatlingComple_{ false };
};
#endif // !ENEMY_BULLET_CONTROL_H_