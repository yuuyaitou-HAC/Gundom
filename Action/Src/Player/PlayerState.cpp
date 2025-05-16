#include "PlayerState.h"
#include "Player.h"

//初期化
void PlayerState::initialize_state_() {

	//攻撃力
	AttackValue_ = 30;
	//防御力
	DefenseValue_ = 20;
	//HP
	HPValue_ = 100;
	//最大HP
	MaxHPValue_ = 100;
	//歩行速度
	WalkSpeed_ = 0.15f;//0.15
	//エネルギー
	EnergyValue_ = 100.0f;

	MaxEnergyValue_ = 100.0f;

	//各種弾
	BeamBullet_ = 20;
	BeamMagnamBullet_ = 7;
	BazookaBullet_ = 3;

	//各マガジン
	BeamMagnamMagazin_ = 3;
	BazookaMagazin_ = 3;

	//EXスキルポイント
	ExSkillPoint_ = 0;
}

//補給
void PlayerState::supply() {

	//HPの回復
	AddHP(maxHP() - hp());

	//エネルギー
	addEnargy(100 - enargy());

	//ビームライフルの弾
	setBeamBullet(20 - beamBullet());

	//ビームマグナムの弾
	setBeamMagnumBullet(7 - beamMagnumBullet());
	setBeamMagnamMagazin(3 - beamMagnamMagazin());

	//バズーカの弾
	setBazookaBullet(3 - bazookaBullet());
	setBazookaMagazin(3 - bazookaMagazin());
}

//攻撃力を返す
int PlayerState::attack() const {
	return AttackValue_;
}

//攻撃力の変更
void PlayerState::addAttack(int changeAT) {
	AttackValue_ += changeAT;
}

//防御力を返す
int PlayerState::defense() const {
	return DefenseValue_;
}

//防御力の変更
void PlayerState::addDefense(int changeDE) {
	DefenseValue_ += changeDE;
}

//HPを返す
int PlayerState::hp() const {
	return HPValue_;
}

//HPの変更
void PlayerState::AddHP(int changeHP) {
	HPValue_ += changeHP;
}

//MaxHpを返す
int PlayerState::maxHP() const {
	return MaxHPValue_;
}

//MAXHPの変更
void PlayerState::addMaxHP(int changeMaxHP) {
	MaxHPValue_ += changeMaxHP;
}

//移動速度の返す
float PlayerState::moveSpeed() const {
	return WalkSpeed_;
}

//移動速度の変更
void PlayerState::addMoveS(float changeMS) {
	float ms = changeMS / 100.0f + 1.0f;

	WalkSpeed_ *= ms;
}

//エネルギー量を返す
float PlayerState::enargy() const {
	return EnergyValue_;
}

//エネルギー量の変更
void PlayerState::addEnargy(float changeE) {
	EnergyValue_ += changeE;
	EnergyValue_ = CLAMP(EnergyValue_, 0.0f, MaxEnergyValue_);
}

float PlayerState::MaxEnargy() const {
	return MaxEnergyValue_;
}

void PlayerState::addMaxEnargy(float maxenargy) {
	MaxEnergyValue_ += maxenargy;
}

//銃の種類を返す
PlayerState::GunState PlayerState::gunstate_()const {
	return GS;
}

//銃の種類を変更
void PlayerState::setGunState(GunState gunstate) {
	GS = gunstate;
}

//ビームライフルの弾を返す
int PlayerState::beamBullet() const {
	return BeamBullet_;
}

//ビームライフルの弾変更
void PlayerState::setBeamBullet(int Bullet) {
	BeamBullet_ += Bullet;
}

//ビームマグナムの弾を返す
int PlayerState::beamMagnumBullet()const {
	return BeamMagnamBullet_;
}

//ビームマグナムの弾変更
void PlayerState::setBeamMagnumBullet(int Bullet) {
	BeamMagnamBullet_ += Bullet;
}

//バズーカの弾を返す
int PlayerState::bazookaBullet() const {
	return BazookaBullet_;
}

//バズーカの弾の変更
void PlayerState::setBazookaBullet(int Bullet) {
	BazookaBullet_ += Bullet;

}

//ビームマグナムのマガジン数を返す
int PlayerState::beamMagnamMagazin() const {
	return BeamMagnamMagazin_;
}

//ビームマグナムのマガジン数を変更
void PlayerState::setBeamMagnamMagazin(int Magazin) {
	BeamMagnamMagazin_ += Magazin;
}

//バズーカのマガジン数を返す
int PlayerState::bazookaMagazin()const {
	return BazookaMagazin_;
}

//バズーカのマガジン数を変更
void PlayerState::setBazookaMagazin(int Magazin) {
	BazookaMagazin_ += Magazin;
}

//EXスキルポイントを返す
int PlayerState::exSkillPoint() const {
	return ExSkillPoint_;
}

//EXスキルポイントの変更
void PlayerState::setExSkillPoint(int point) {
	ExSkillPoint_ += point;
	//EXスキルポイントを一定値内に抑える
	ExSkillPoint_ = CLAMP(ExSkillPoint_, 0, 300);
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