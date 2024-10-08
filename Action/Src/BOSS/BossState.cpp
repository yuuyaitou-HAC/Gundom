#include "BossState.h"
//#include "Boss.h"

//攻撃力
int BossAttackValue_ = 20;

//防御力
int BossDefenseValue_ = 20;

//HP
int BossHPValue_ = 100;

//最大HP	
int BossMaxHPValue_ = 100;

//元のスピード
float BossWalkSpeed_ = 0.15f;

//エネルギー量
float BossEnergyValue_ = 100.0f;

//ビームライフルの弾
int BossBB = 20;

//ガトリングの弾
float GB = 180.0f;

//バスターライフルの弾
int BLB = 1;


void BossState::initialize_state_() {

	//攻撃力
	BossAttackValue_ = 20;
	//防御力
	BossDefenseValue_ = 20;
	//HP
	BossHPValue_ = 100;
	//最大HP	
	BossMaxHPValue_ = 100;
	//歩行速度
	BossWalkSpeed_ = 0.15f;

	BossEnergyValue_ = 100.0f;

}

int BossState::Attack() const {

	return BossAttackValue_;
}

void BossState::AddAttack(int changeAT) {

	BossAttackValue_ += changeAT;

}

int BossState::Defense() const {

	return BossDefenseValue_;
}

void BossState::AddDefense(int changeDE) {

	BossDefenseValue_ += changeDE;

}

int BossState::HP() const
{
	return BossHPValue_;
}

void BossState::AddHP(int changeHP) {

	BossHPValue_ += changeHP;

}


int BossState::MaxHP() const
{
	return BossMaxHPValue_;
}

void BossState::AddMaxHP(int changeMaxHP) {

	BossMaxHPValue_ += changeMaxHP;

}

float BossState::MoveSpeed() const
{
	return BossWalkSpeed_;
}

void BossState::AddMoveS(float changeMS) {

	float ms = changeMS / 100.0f + 1.0f;

	BossWalkSpeed_ *= ms;

}

float BossState::Enargy() const {

	return BossEnergyValue_;
}

void BossState::AddEnargy(float changeE) {

	BossEnergyValue_ += changeE;

}

BossState::GunState BossState::gunstate_() {

	return GS;

}

void BossState::SetGunState(GunState gunstate) {

	GS = gunstate;

}

int BossState::BeamBullet() {

	return BossBB;

}

void BossState::SetBeamBullet(int Bullet) {

	BossBB += Bullet;

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
