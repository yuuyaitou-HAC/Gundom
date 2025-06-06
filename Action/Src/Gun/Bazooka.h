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

private:

	void Cool();

private:

	Player* player_;

private:

	//残弾数
	int nowMagazine_;

	//残弾数(代入)
	int assignmentMagazine_;

	float coolTime_;

	float assignmentCoolTime_;

	bool coolTimeTriger_;

	float deltaTimer_;

	int magazin_;
};
#endif // !BEAM_MAGNUM_H_