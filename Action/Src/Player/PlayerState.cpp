#include "PlayerState.h"
#include "Player.h"

//攻撃力
int AttackValue_ = 20;

//防御力
int DefenseValue_ = 20;

//HP
int HPValue_ = 100;

//最大HP	
int MaxHPValue_ = 100;

//元のスピード
float WalkSpeed_ = 0.15f;

//エネルギー量
float EnergyValue_ = 100.0f;

void PlayerState::initialize_state_(){

	//攻撃力
	AttackValue_ = 20;
	//防御力
	DefenseValue_ = 20;
	//HP
	HPValue_ = 100;
	//最大HP	
	MaxHPValue_ = 100;
	//歩行速度
	WalkSpeed_ = 0.15f;

	EnergyValue_ = 100.0f;

}

int PlayerState::Attack() const
{
    return AttackValue_;
}

void PlayerState::AddAttack(int changeAT)
{

	AttackValue_ += changeAT;

}

int PlayerState::Defense() const
{
    return DefenseValue_;
}

void PlayerState::AddDefense(int changeDE)
{

	DefenseValue_ += changeDE;

}

int PlayerState::HP() const
{
    return HPValue_;
}

void PlayerState::AddHP(int changeHP)
{

	HPValue_ += changeHP;

}

int PlayerState::MaxHP() const
{
    return MaxHPValue_;
}

void PlayerState::AddMaxHP(int changeMaxHP)
{

	MaxHPValue_ += changeMaxHP;

}

float PlayerState::MoveSpeed() const
{
    return WalkSpeed_;
}

void PlayerState::AddMoveS(float changeMS)
{
	float ms = changeMS / 100.0f + 1.0f;

	WalkSpeed_ *= ms;

}

float PlayerState::Enargy() const
{
    return EnergyValue_;
}

void PlayerState::AddEnargy(float changeE)
{

	EnergyValue_ += changeE;

}
