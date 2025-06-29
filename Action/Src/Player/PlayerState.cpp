#include "PlayerState.h"
#include "Player.h"

//初期化
void PlayerState::initialize_state_() {

	//攻撃力
	attackValue_ = attackValueInitialize_;
	//防御力
	defenseValue_ = defenseValueInitialize_;
	//HP
	hpValue_ = hpValueInitialize_;
	//最大HP
	maxHPValue_ = maxHPValueInitialize_;
	//歩行速度
	walkSpeed_ = walkSpeedInitialize_;
	//エネルギー
	energyValue_ = energyValueInitialize_;

	maxEnergyValue_ = maxEnergyValueInitialize_;

	//各種弾
	beamBullet_ = beamBulletInitialize_;
	beamMagnamBullet_ = beamMagnamBulletInitialize_;
	bazookaBullet_ = bazookaBulletInitialize_;

	//各マガジン
	beamMagnamMagazin_ = beamMagnamMagazinInitialize_;
	bazookaMagazin_ = bazookaMagazin_;

	//EXスキルポイント
	exSkillPoint_ = 0;
}

//攻撃力を返す
int PlayerState::attack() const {
	return attackValue_;
}

//攻撃力の変更
void PlayerState::addAttack(int changeAT) {
	attackValue_ += changeAT;
}

//防御力を返す
int PlayerState::defense() const {
	return defenseValue_;
}

//防御力の変更
void PlayerState::addDefense(int changeDE) {
	defenseValue_ += changeDE;
}

//HPを返す
int PlayerState::hp() const {
	return hpValue_;
}

//HPの変更
void PlayerState::AddHP(int changeHP) {
	hpValue_ += changeHP;
}

//MaxHpを返す
int PlayerState::maxHP() const {
	return maxHPValue_;
}

//MAXHPの変更
void PlayerState::addMaxHP(int changeMaxHP) {
	maxHPValue_ += changeMaxHP;
}

//移動速度の返す
float PlayerState::moveSpeed() const {
	return walkSpeed_;
}

//移動速度の変更
void PlayerState::addMoveS(float changeMS) {
	float ms = changeMS / 100.0f + 1.0f;

	walkSpeed_ *= ms;
}

//エネルギー量を返す
float PlayerState::enargy() const {
	return energyValue_;
}

//エネルギー量の変更
void PlayerState::addEnargy(float changeE) {
	energyValue_ += changeE;
	energyValue_ = CLAMP(energyValue_, 0.0f, maxEnergyValue_);
}

float PlayerState::MaxEnargy() const {
	return maxEnergyValue_;
}

void PlayerState::addMaxEnargy(float maxenargy) {
	maxEnergyValue_ += maxenargy;
}

//銃の種類を返す
PlayerState::GunState PlayerState::gunstate_()const {
	return gs_;
}

//銃の種類を変更
void PlayerState::setGunState(GunState gunstate) {
	gs_ = gunstate;
}

//ビームライフルの弾を返す
int PlayerState::beamBullet() const {
	return beamBullet_;
}

//ビームライフルの弾変更
void PlayerState::setBeamBullet(int Bullet) {
	beamBullet_ += Bullet;
}

//ビームマグナムの弾を返す
int PlayerState::beamMagnumBullet()const {
	return beamMagnamBullet_;
}

//ビームマグナムの弾変更
void PlayerState::setBeamMagnumBullet(int Bullet) {
	beamMagnamBullet_ += Bullet;
}

//バズーカの弾を返す
int PlayerState::bazookaBullet() const {
	return bazookaBullet_;
}

//バズーカの弾の変更
void PlayerState::setBazookaBullet(int Bullet) {
	bazookaBullet_ += Bullet;

}

//ビームマグナムのマガジン数を返す
int PlayerState::beamMagnamMagazin() const {
	return beamMagnamMagazin_;
}

//ビームマグナムのマガジン数を変更
void PlayerState::setBeamMagnamMagazin(int Magazin) {
	beamMagnamMagazin_ += Magazin;
}

//バズーカのマガジン数を返す
int PlayerState::bazookaMagazin()const {
	return bazookaMagazin_;
}

//バズーカのマガジン数を変更
void PlayerState::setBazookaMagazin(int Magazin) {
	bazookaMagazin_ += Magazin;
}

//EXスキルポイントを返す
int PlayerState::exSkillPoint() const {
	return exSkillPoint_;
}

//EXスキルポイントの変更
void PlayerState::addExSkillPoint(int point) {
	exSkillPoint_ += point;
	//EXスキルポイントを一定値内に抑える
	exSkillPoint_ = CLAMP(exSkillPoint_, 0, 300);
}

void PlayerState::setEXSkill(float magnification) {

	exMag_ = magnification - 1;

	//発動直前のステータスを保存
	nowAttackValue_ = attack() * exMag_;
	nowDefenceValue_ = defense() * exMag_;
	nowSpeed_ = moveSpeed() * exMag_;
	nowEnargy_ = MaxEnargy() * exMag_;


	//ステータス上昇
	addAttack(nowAttackValue_);
	addDefense(nowDefenceValue_);
	addMoveS(nowSpeed_);
	addMaxEnargy(nowEnargy_);
}

void PlayerState::resetEXSkill() {

	//EXスキルで増えた値をもとに戻す
	addAttack(-nowAttackValue_);
	addDefense(-nowDefenceValue_);
	addMoveS(-nowSpeed_);
	addMaxEnargy(-nowEnargy_);

	nowAttackValue_ = 0;
	nowDefenceValue_ = 0;
	nowSpeed_ = 0;
	nowEnargy_ = 0;
}