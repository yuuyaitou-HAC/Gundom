#ifndef BOSS_BASTERLIFLE_H_
#define BOSS_BASTERLIFLE_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include <gslib.h>

class Boss;

class Player;

class BossBasterRifle : public Actor {

public:

	//コンストラクタ
	BossBasterRifle(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//射撃
	void fire();

private:

	Boss* boss;
	Player* player;

private:

	//アップデートで一回だけ呼ぶ
	bool FarstUpdate_;

};

#endif // !BOSS_BASTERLIFLE_H_
