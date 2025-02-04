#ifndef ENEMY_BULLET_CONTROL_H_
#define ENEMY_BULLET_CONTROL_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "EnemyAI/HBMAI.h"
#include "EnemyAI/TankAI.h"

class EnemyBulletControl :public Actor {

public:

	//コンストラクタ
	EnemyBulletControl(IWorld* world, const GSvector3& position);
	
	//デストラクタ
	~EnemyBulletControl();

	//更新
	virtual void update(float delta_time)override;

	virtual void draw() const override;

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
	std::vector<HBMAI*> BeamLifleAIs_;
	std::vector<HBMAI*> GatlingAIs_;
	std::vector<TankAI*>TankAIs_;

	//呼び出すAIを入れる変数
	HBMAI* hbmai1_;
	HBMAI* hbmai2_;

	TankAI* tankai1_;
	TankAI* tankai2_;

	//呼び出す番号
	int BeamLifleAICallNumber = 0;
	int GatringAICallNumber = 0;
	int TankAICallNumber = 0;

	//弾切れ起こした個体をカウント
	int hbmcounter;
	int tankcounter;

	int callcounter;

	//クールタイム
	float BeamLifleAttackTime = 180.0f;
	float GatringAttackTime = 180.0f;
	float TankAttackTime = 120.0f;

	//NULLの時の時間
	float hbmprocessingTimer1 = 300.0f;
	float hbmprocessingTimer2 = 300.0f;

	//NULLの時の時間
	float tankprocessingTimer1 = 300.0f;
	float tankprocessingTimer2 = 300.0f;

	//呼び出した個体の状態に応じて処理を変える
	bool hbmcall1;
	bool hbmcall2;

	bool tankcall1;
	bool tankcall2;
};
#endif // !ENEMY_BULLET_CONTROL_H_