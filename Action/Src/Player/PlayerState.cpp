#include "PlayerState.h"
#include "Player.h"

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
int BeamBullet_ = 20;

//ビームマグナムの弾
int BeamMagnamBullet_ = 7;

//ビームマグナムのマガジン数
int BeamMagnamMagazin_ = 3;

//バズーカの弾
int BazookaBullet_ = 3;

//バズーカのマガジン数
int BazookaMagazin_ = 3;

//初期化
void PlayerState::initialize_state_() {

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
	//エネルギー
	EnergyValue_ = 100.0f;

	//各種弾
	BeamBullet_ = 20;
	BeamMagnamBullet_ = 7;
	BazookaBullet_ = 3;

	//各マガジン
	BeamMagnamMagazin_ = 3;
	BazookaMagazin_ = 3;
}

//補給
void PlayerState::supply() {

	//HPの回復
	AddHP(MaxHP() - HP());

	//エネルギー
	AddEnargy(100 - Enargy());

	//ビームライフルの弾
	SetBeamBullet(20 - BeamBullet());

	//ビームマグナムの弾
	SetBeamMagnumBullet(7 - BeamMagnumBullet());
	SetBeamMagnamMagazin(3 - BeamMagnamMagazin());

	//バズーカの弾
	SetBazookaBullet(3 - BazookaBullet());
	SetBazookaMagazin(3 - BazookaMagazin());
}

//攻撃力を返す
int PlayerState::Attack() const {

	return AttackValue_;
}

//攻撃力の変更
void PlayerState::AddAttack(int changeAT) {

	AttackValue_ += changeAT;
}

//防御力を返す
int PlayerState::Defense() const {

	return DefenseValue_;
}

//防御力の変更
void PlayerState::AddDefense(int changeDE) {

	DefenseValue_ += changeDE;
}

//HPを返す
int PlayerState::HP() const {

	return HPValue_;
}

//HPの変更
void PlayerState::AddHP(int changeHP) {

	HPValue_ += changeHP;
}

//MaxHpを返す
int PlayerState::MaxHP() const {

	return MaxHPValue_;
}

//MAXHPの変更
void PlayerState::AddMaxHP(int changeMaxHP) {

	MaxHPValue_ += changeMaxHP;
}

//移動速度の返す
float PlayerState::MoveSpeed() const {

	return WalkSpeed_;
}

//移動速度の変更
void PlayerState::AddMoveS(float changeMS) {

	float ms = changeMS / 100.0f + 1.0f;

	WalkSpeed_ *= ms;
}

//エネルギー量を返す
float PlayerState::Enargy() const {

	return EnergyValue_;
}

//エネルギー量の変更
void PlayerState::AddEnargy(float changeE) {

	EnergyValue_ += changeE;
}

//銃の種類を返す
PlayerState::GunState PlayerState::gunstate_() {

	return GS;
}

//銃の種類を変更
void PlayerState::SetGunState(GunState gunstate) {

	GS = gunstate;
}

//ビームライフルの弾を返す
int PlayerState::BeamBullet() {

	return BeamBullet_;
}

//ビームライフルの弾変更
void PlayerState::SetBeamBullet(int Bullet) {

	BeamBullet_ += Bullet;
}

//ビームマグナムの弾を返す
int PlayerState::BeamMagnumBullet() {

	return BeamMagnamBullet_;
}

//ビームマグナムの弾変更
void PlayerState::SetBeamMagnumBullet(int Bullet) {

	BeamMagnamBullet_ += Bullet;
}

//バズーカの弾を返す
int PlayerState::BazookaBullet() {

	return BazookaBullet_;
}

//バズーカの弾の変更
void PlayerState::SetBazookaBullet(int Bullet) {

	BazookaBullet_ += Bullet;

}

//ビームマグナムのマガジン数を返す
int PlayerState::BeamMagnamMagazin() {

	return BeamMagnamMagazin_;
}

//ビームマグナムのマガジン数を変更
void PlayerState::SetBeamMagnamMagazin(int Magazin) {
	BeamMagnamMagazin_ += Magazin;
}

//バズーカのマガジン数を返す
int PlayerState::BazookaMagazin() {

	return BazookaMagazin_;
}

//バズーカのマガジン数を変更
void PlayerState::SetBazookaMagazin(int Magazin) {
	BazookaMagazin_ += Magazin;
}
