#ifndef BOSS_H_
#define BOSS_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

class Player;

class Boss : public Actor {

public:

	enum class State {
		Idle,		//アイドル
		Move,		//移動
		Shooting,	//射撃
		Slashing,	//斬撃
		Damage,		//ダメージ
		Die,		//死
	};

public:

	Boss(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

	
};

#endif // !BOSS_H_
