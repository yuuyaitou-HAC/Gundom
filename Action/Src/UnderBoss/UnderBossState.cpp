#include "UnderBossState.h"
#include "UnderBoss.h"

//攻撃力
int BossAttackValue_ = 20;

//防御力
int BossDefenseValue_ = 20;

//HP
int BossHPValue_ = 100;

//最大HP	
int BossMaxHPValue_ = 100;

//元のスピード
float BossWalkSpeed_ = 0.5f;

//エネルギー量
float BossEnergyValue_ = 100.0f;

//ビームライフルの弾
int BossBB_ = 20;

//ガトリングの弾
int GB_ = 200;

//バスターライフルの弾
int BLB_ = 1;

bool BasterFrag_ = false;

void UnderBossState::initialize_state_() {

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

//攻撃力参照
int UnderBossState::Attack() const {
	return BossAttackValue_;
}

//攻撃力変更
void UnderBossState::AddAttack(int changeAT) {
	BossAttackValue_ += changeAT;
}

//防御力参照
int UnderBossState::Defense() const {
	return BossDefenseValue_;
}

//防御力変更
void UnderBossState::AddDefense(int changeDE) {
	BossDefenseValue_ += changeDE;
}

//HP参照
int UnderBossState::HP() const {
	return BossHPValue_;
}

//HP変更
void UnderBossState::AddHP(int changeHP) {
	BossHPValue_ += changeHP;
}

//MAXHP参照
int UnderBossState::MaxHP() const {
	return BossMaxHPValue_;
}

//MAXHP変更
void UnderBossState::AddMaxHP(int changeMaxHP) {
	BossMaxHPValue_ += changeMaxHP;
}

//移動速度参照
float UnderBossState::MoveSpeed() const {
	return BossWalkSpeed_;
}

//移動速度変更
void UnderBossState::AddMoveS(float changeMS) {
	float ms = changeMS / 100.0f + 1.0f;
	BossWalkSpeed_ *= ms;
}

//エネルギー参照
float UnderBossState::Enargy() const {
	return BossEnergyValue_;
}

//エネルギー変更
void UnderBossState::AddEnargy(float changeE) {
	BossEnergyValue_ += changeE;
}

//銃のステータス参照
UnderBossState::GunState UnderBossState::gunstate_()const {
	return GS;
}

//銃のステータス変更
void UnderBossState::SetGunState(GunState gunstate) {
	GS = gunstate;
}

//ビームライフルの弾参照
int UnderBossState::BeamBullet() const {
	return BossBB_;
}

//ビームライフルの弾変更
void UnderBossState::SetBeamBullet(int Bullet) {
	BossBB_ += Bullet;
}

//ガトリングの弾参照
int UnderBossState::GatlingBullet() const {
	return GB_;
}

//ガトリングの弾変更
void UnderBossState::SetGatlingBullet(int Bullet) {
	GB_ += Bullet;
}

//バスターライフルの弾参照
int UnderBossState::BasterBullet() const {
	return BLB_;
}

//バスターライフルの弾変更
void UnderBossState::SetBasterBullet(int Bullet) {
	BLB_ += Bullet;
}

//バスターライフルのフラグ参照
bool UnderBossState::BasterFrag()const {
	return BasterFrag_;
}

//バスターライフルのフラグ変更
void UnderBossState::SetBasterFrag(bool frag) {
	BasterFrag_ = frag;
}