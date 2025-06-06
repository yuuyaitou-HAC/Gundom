#ifndef BEAMUGUN_H_
#define BEAMUGUN_H_

#include "Actor\Actor.h"
#include "Actor\AnimationMesh.h"
#include <gslib.h>

class Player;

class GunControl;

class BeamGun :public Actor {

public:

	//コンストラクタ
	BeamGun(IWorld* world, const GSvector3& position);
	
	//更新
	virtual void update(float delta_time)override;

	//発砲
	void Fire();

private:

	//クールタイム
	void Cool();

private:

	Player* player_;

	GunControl* gunControl_;

private:

	//残弾数
	int nowMagazine_;

	//残弾数(代入)
	int BazookaMagazine_;

	float coolTime_;

	float BazookaCoolTime_;

	bool coolTimeTriger_;

	float deltaTimer_;
};
#endif // !BEAMUGUN_H_