#ifndef BASIC_ATTACK_COLLIDER_H_
#define BASIC_ATTACK_COLLIDER_H_

#include "Actor/Actor.h"


class BasicAttackCollider : public Actor {

public:
	BasicAttackCollider() = default;
	~BasicAttackCollider() = default;

	virtual void update(float delta_time)override {};
	virtual void draw()const override {};
	virtual void draw_gui()const override {};
	virtual void react(Actor& other)override {};
	virtual void handle_message(const std::string& message, void* param)override {};

public:

	//UŒ‚—Í‚Ìæ“¾
	int GetAttackValue() const;
	
protected:
	int m_AttackValue{ 0 };
};


#endif // !1
