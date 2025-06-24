#include "BossState.h"

//ƒXƒe[ƒ^ƒX‚Ì‰Šú‰»
void BossState::initialize_state_() {
	HP_ = initializeHP_;
	MaxHP_ = initializeMaxHP_;
	attack_ = initializeAttack_;
	defense_ = initializeDefense_;
	speed_ = initializeSpeed_;
}

//HP‚ğ•Ô‚·
int BossState::HP() const {
	return HP_;
}

//HP‚Ì‰ÁZ
void BossState::AddHP(int value) {
	HP_ += value;
	HP_ = CLAMP(HP_, 0, MaxHP_);
}

//MaxHP‚ğ•Ô‚·
int BossState::MaxHP() const {
	return MaxHP_;
}

//MaxHP‚Ì‰ÁZ
void BossState::AddMaxHp(int value) {
	MaxHP_ += value;
}

//–hŒä—Í‚ğ•Ô‚·
int BossState::defense() const {
	return defense_;
}

//–hŒä—Í‚Ì‰ÁZ
void BossState::addDefense(int value) {
	defense_ += value;
}

//UŒ‚—Í‚ğ•Ô‚·
int BossState::attack() const {
	return attack_;
}

//UŒ‚—Í‚Ì‰ÁZ
void BossState::addAttack(int value) {
	attack_ += value;
}

//ˆÚ“®‘¬“x‚ğ•Ô‚·
float BossState::moveSpeed() const {
	return speed_;
}

//ˆÚ“®‘¬“x‚Ì‰ÁZ
void BossState::addMoveS(float value) {

	float ms = value / 100.0f + 1.0f;
	speed_ *= ms;
}