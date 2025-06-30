#include "UnderBossState.h"
#include "UnderBoss.h"

void UnderBossState::initialize_state_() {

	//攻撃力
	underBossAttackValue_ = assignmentUnderBossAttackValue_;
	//防御力
	underBossDefenseValue_ = assignmentUnderBossDefenseValue_;
	//HP
	underBossHPValue_ = assignmentUnderBossHPValue_;
	//最大HP	
	underBossMaxHPValue_ = assignmentUnderBossMaxHPValue_;
	//歩行速度
	underBossWalkSpeed_ = assignmentUnderBossWalkSpeed_;

	underBossEnergyValue_ = assignmentUnderBossEnergyValue_;

	underBossBB_ = assignmentUnderBossBB_;

	GB_ = assignmentGB_;

	BLB_ = assignmentBLB_;

	BasterFrag_ = false;
}

//攻撃力参照
int UnderBossState::Attack() const {
	return underBossAttackValue_;
}

//攻撃力変更
void UnderBossState::AddAttack(int changeAT) {
	underBossAttackValue_ += changeAT;
}

//防御力参照
int UnderBossState::Defense() const {
	return underBossDefenseValue_;
}

//防御力変更
void UnderBossState::AddDefense(int changeDE) {
	underBossDefenseValue_ += changeDE;
}

//HP参照
int UnderBossState::HP() const {
	return underBossHPValue_;
}

//HP変更
void UnderBossState::AddHP(int changeHP) {
	underBossHPValue_ += changeHP;
	underBossHPValue_ = CLAMP(underBossHPValue_, 0, underBossMaxHPValue_);
}

//MAXHP参照
int UnderBossState::MaxHP() const {
	return underBossMaxHPValue_;
}

//MAXHP変更
void UnderBossState::AddMaxHP(int changeMaxHP) {
	underBossMaxHPValue_ += changeMaxHP;
}

//移動速度参照
float UnderBossState::MoveSpeed() const {
	return underBossWalkSpeed_;
}

//移動速度変更
void UnderBossState::AddMoveS(float changeMS) {
	float ms = changeMS / 100.0f + 1.0f;
	underBossWalkSpeed_ *= ms;
}

//エネルギー参照
float UnderBossState::Enargy() const {
	return underBossEnergyValue_;
}

//エネルギー変更
void UnderBossState::AddEnargy(float changeE) {
	underBossEnergyValue_ += changeE;
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
	return underBossBB_;
}

//ビームライフルの弾変更
void UnderBossState::SetBeamBullet(int Bullet) {
	underBossBB_ += Bullet;
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