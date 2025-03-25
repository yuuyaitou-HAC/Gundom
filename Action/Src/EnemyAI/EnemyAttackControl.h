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
	void attackGatling(float delta_time);
	void attackTanck(float delta_time);

private:

	//各AIを格納する配列
	std::vector<HBMAI*> beamLifleAIs_;
	std::vector<HBMAI*> gatlingAIs_;
	std::vector<TankAI*>tankAIs_;

	//呼び出すAIを入れる変数
	HBMAI* beamRifleAI1_;
	HBMAI* beamRifleAI2_;

	HBMAI* GatringAI_;

	TankAI* tankai1_;
	TankAI* tankai2_;

	//ビームライフル

		//呼び出す個体の番号
	int beamLifleAICallNumber1_;
	int beamLifleAICallNumber2_;

	//次の呼び出し処理までの時間
	float beamLifleAttackTime_ = 180.0f;

	//割り当てられた個体がNULLかどうか
	bool beamLifleNULL1_;
	bool beamLifleNULL2_;

	//射撃の処理が終了したか
	bool beamLifleComple_;
	bool beamLifleComple2_;

	//呼び出し個体の割り当て終了したか
	bool beamLifleCallComple_ = false;

	//戦車

	int tankAICallNumber1_;
	int tankAICallNumber2_;

	//次の呼び出し処理までの時間
	float tankLifleAttackTime_ = 180.0f;

	//割り当てられた個体がNULLかどうか
	bool tankNULL1_;
	bool tankNULL2_;

	//射撃の処理が終了したか
	bool tankComple1_;
	bool tankComple2_;

	//呼び出し個体の割り当て終了したか
	bool tankCallComple_ = false;

	//ガトリング

	//呼び出し番号
	int gatringAICallNumber_ = 0;

	//次の呼び出しまでの時間
	float gatringAttackTime_ = 180.0f;

	//呼び出した対象がNULLの場合
	bool gatringNULL_;

	//射撃終了したら
	bool gatringComple_;
};
#endif // !ENEMY_BULLET_CONTROL_H_