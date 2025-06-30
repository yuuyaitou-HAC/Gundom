#ifndef UNDER_BOSS_BASTERLIFLE_H_
#define UNDER_BOSS_BASTERLIFLE_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include <gslib.h>

class UnderBoss;

class Player;

class UnderBossBasterRifle : public Actor {

public:

	//コンストラクタ
	UnderBossBasterRifle(IWorld* world, const GSvector3& position, const UnderBoss* underBoss);

	//更新
	virtual void update(float delta_time)override;

	//射撃
	void fire();

private:

	const UnderBoss* underBoss_{ NULL };

	Player* player_{ NULL };

	//生成時の調整
	const float makePosOffset_{ 1.5f };

	//弾速
	const float speed_{ 0.5f };

	const int attackValue_{ 5 };

};
#endif // !BOSS_BASTERLIFLE_H_