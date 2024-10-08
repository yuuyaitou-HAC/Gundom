#include "BossState.h"
//#include "Boss.h"

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

//ビームライフルの弾
int BB = 20;

//ガトリングの弾
float GB = 180.0f;

//バスターライフルの弾
int BLB = 1;


void BossState::initialize_state_() {

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

int BossState::Attack() const {

	return AttackValue_;
}

void BossState::AddAttack(int changeAT) {

	AttackValue_ += changeAT;

}

int BossState::Defense() const {

	return DefenseValue_;
}

void BossState::AddDefense(int changeDE) {

	DefenseValue_ += changeDE;

}

int BossState::HP() const
{
	return HPValue_;
}

void BossState::AddHP(int changeHP) {

	HPValue_ += changeHP;

}


int BossState::MaxHP() const
{
	return MaxHPValue_;
}

void BossState::AddMaxHP(int changeMaxHP) {

	MaxHPValue_ += changeMaxHP;

}

float BossState::MoveSpeed() const
{
	return WalkSpeed_;
}

void BossState::AddMoveS(float changeMS) {

	float ms = changeMS / 100.0f + 1.0f;

	WalkSpeed_ *= ms;

}

float BossState::Enargy() const {

	return EnergyValue_;
}

void BossState::AddEnargy(float changeE) {

	EnergyValue_ += changeE;

}

BossState::GunState BossState::gunstate_() {

	return GS;

}

void BossState::SetGunState(GunState gunstate) {

	GS = gunstate;

}

int BossState::BeamBullet() {

	return BB;

}

void BossState::SetBeamBullet(int Bullet) {

	BB += Bullet;

}

float BossState::GatlingBullet() {

	return GB;
}

void BossState::SetGatlingBullet(float Bullet) {

	GB += Bullet;

}

int BossState::BasterBullet() {

	return BLB;
}

void BossState::SetBasterBullet(int Bullet) {

	BLB += Bullet;

}
