#ifndef BAZOOKA_H_
#define BAZOOKA_H_

#include "Actor\Actor.h"
#include "Actor\AnimationMesh.h"
#include <gslib.h>

class Player;

class Bazooka :public Actor {

public:

	//コンストラクタ
	Bazooka(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	void Fire();

	int Test();

private:

	Player* player_;

private:

	//残弾数
	int NowMagazine;

	//残弾数(代入)
	int AsignmentMagazine;

	float CoolTime;

	float AsignmentCoolTime;

	bool CoolTimeTriger;

	float delta_timer;

	void Cool();

	int Magazin;

};

#endif // !BEAM_MAGNUM_H_
