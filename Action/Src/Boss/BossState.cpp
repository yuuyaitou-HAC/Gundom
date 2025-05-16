#include "BossState.h"

//HP
int HP_ = 1000;

//MaxHP
int MaxHP_ = 1000;

//攻撃力
int attack_ = 20;

//防御力
int defense_ = 20;

//移動速度
float speed_ = 0.3f;

//ステータスの初期化
void BossState::initialize_state_() {
	HP_ = 1000;
	MaxHP_ = 1000;
	attack_ = 20;
	defense_ = 20;
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