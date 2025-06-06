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
	UnderBossBasterRifle(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//射撃
	void fire();

private:

	UnderBoss* boss_;
	Player* player_;

private:

	//アップデートで一回だけ呼ぶ
	bool onrTrigger_;
};

#endif // !BOSS_BASTERLIFLE_H_