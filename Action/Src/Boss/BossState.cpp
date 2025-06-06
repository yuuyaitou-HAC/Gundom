#include "BossState.h"

//ステータスの初期化
void BossState::initialize_state_() {
	HP_ = 1000;
	MaxHP_ = 1000;
	attack_ = 50;
	defense_ = 25;
	speed_ = 0.3f;
}

int BossState::HP() const {
	return HP_;
}

void BossState::AddHP(int value) {
	HP_ += value;
	HP_ = CLAMP(HP_, 0, MaxHP_);
}

int BossState::MaxHP() const {
	return MaxHP_;
}

void BossState::AddMaxHp(int value) {
	MaxHP_ += value;
}

int BossState::defense() const {
	return defense_;
}

void BossState::addDefense(int value) {
	defense_ += value;
}

int BossState::attack() const {
	return attack_;
}

void BossState::addAttack(int value) {
	attack_ += value;
}

float BossState::moveSpeed() const {
	return speed_;
}

void BossState::addMoveS(float value) {

	float ms = value / 100.0f + 1.0f;

	speed_ *= ms;
}