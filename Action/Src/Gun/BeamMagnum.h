#ifndef BEAM_MAGNUM_H_
#define BEAM_MAGNUM_H_

#include "Actor\Actor.h"
#include "Actor\AnimationMesh.h"
#include <gslib.h>

class Player;

class BeamMagnum :public Actor {

public:

	//コンストラクタ
	BeamMagnum(IWorld* world, const GSvector3& position);

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
	int BazookaMagazine_;

	float coolTime_;

	float BazookaCoolTime_;

	bool coolTimeTriger_;

	float deltaTimer_;

	int magazin_;
};
#endif // !BEAM_MAGNUM_H_