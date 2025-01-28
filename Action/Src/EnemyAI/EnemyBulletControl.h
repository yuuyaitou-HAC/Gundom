#ifndef ENEMY_BULLET_CONTROL_H_
#define ENEMY_BULLET_CONTROL_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "BattleShip/EnemyShip.h"
#include "EnemyAI/HBMAI.h"
#include "EnemyAI/TankAI.h"

class EnemyBulletControl :public Actor {

public:

	EnemyBulletControl(IWorld* world, const GSvector3& position);
	~EnemyBulletControl();

	virtual void update(float delta_time)override;

	//それぞれの部隊を配列に入れるための関数
	void setBeamLifleAI(HBMAI* AI);
	void setGatlingAI(HBMAI* AI);
	void setTanckAI(TankAI* AI);

private:

	void attackBeamLifle(float delta_time);
	void attackGatling(float delta_time);
	void attackTanck(float delta_time);

private:

	EnemyShip* enemyship_;

	//各AIを格納する配列
	std::vector<HBMAI*> BeamLifleAIs_;
	std::vector<HBMAI*> GatlingAIs_;
	std::vector<TankAI*>TankAIs_;

	//各配列数
	const unsigned int BeamLifleAINumberOfArrays = 5;
	const unsigned int GatlingAINumberOfArrays = 2;
	const unsigned int TankAINumberOfArrays = 5;

	//呼び出す番号
	int BeamLifleAICallNumber = 0;
	int GatringAICallNumber = 0;
	int TankAICallNumber = 0;

	//クールタイム
	int BeamLifleAttackTime = 180.0f;
	int GatringAttackTime = 180.0f;
	int TankAttackTime = 120.0f;

};

#endif // !ENEMY_BULLET_CONTROL_H_