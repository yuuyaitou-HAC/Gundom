#include "BossState.h"
#include "Boss.h"

//攻撃力
int BossAttackValue_ = 20;

//防御力
int BossDefenseValue_ = 20;

//HP
int BossHPValue_ = 10;

//最大HP	
int BossMaxHPValue_ = 100;

//元のスピード
float BossWalkSpeed_ = 0.25f;

//エネルギー量
float BossEnergyValue_ = 100.0f;

//ビームライフルの弾
int BossBB_ = 20;

//ガトリングの弾
int GB_ = 200;

//バスターライフルの弾
int BLB_ = 1;

bool BasterFrag_ = false;

void BossState::initialize_state_() {

	//攻撃力
	BossAttackValue_ = 20;
	//防御力
	BossDefenseValue_ = 20;
	//HP
	BossHPValue_ = 10;
	//最大HP	
	BossMaxHPValue_ = 100;
	//歩行速度
	BossWalkSpeed_ = 0.15f;

	BossEnergyValue_ = 100.0f;

}

//攻撃力参照
int BossState::Attack() const {

	return BossAttackValue_;
}

//攻撃力変更
void BossState::AddAttack(int changeAT) {

	BossAttackValue_ += changeAT;

}

//防御力参照
int BossState::Defense() const {

	return BossDefenseValue_;
}

//防御力変更
void BossState::AddDefense(int changeDE) {

	BossDefenseValue_ += changeDE;

}

//HP参照
int BossState::HP() const
{
	return BossHPValue_;
}

//HP変更
void BossState::AddHP(int changeHP) {

	BossHPValue_ += changeHP;

}

//MAXHP参照
int BossState::MaxHP() const
{
	return BossMaxHPValue_;
}

//MAXHP変更
void BossState::AddMaxHP(int changeMaxHP) {

	BossMaxHPValue_ += changeMaxHP;

}

//移動速度参照
float BossState::MoveSpeed() const
{
	return BossWalkSpeed_;
}

//移動速度変更
void BossState::AddMoveS(float changeMS) {

	float ms = changeMS / 100.0f + 1.0f;

	BossWalkSpeed_ *= ms;

}

//エネルギー参照
float BossState::Enargy() const {

	return BossEnergyValue_;
}

//エネルギー変更
void BossState::AddEnargy(float changeE) {

	BossEnergyValue_ += changeE;

}

//銃のステータス参照
BossState::GunState BossState::gunstate_() {

	return GS;

}

//銃のステータス変更
void BossState::SetGunState(GunState gunstate) {

	GS = gunstate;

}

//ビームライフルの弾参照
int BossState::BeamBullet() {

	return BossBB_;

}

//ビームライフルの弾変更
void BossState::SetBeamBullet(int Bullet) {

	BossBB_ += Bullet;

}

//ガトリングの弾参照
int BossState::GatlingBullet() {

	return GB_;
}

//ガトリングの弾変更
void BossState::SetGatlingBullet(int Bullet) {

	GB_ += Bullet;

}

//バスターライフルの弾参照
int BossState::BasterBullet() {

	return BLB_;
}

//バスターライフルの弾変更
void BossState::SetBasterBullet(int Bullet) {

	BLB_ += Bullet;

}

//バスターライフルのフラグ参照
bool BossState::BasterFrag()
{
	return BasterFrag_;
}

//バスターライフルのフラグ変更
void BossState::SetBasterFrag(bool frag) {

	BasterFrag_ = frag;

}
