#include "PlayerState.h"
#include "Player.h"

//çUåÇóÕ
int AttackValue_ = 20;

//ñhå‰óÕ
int DefenseValue_ = 20;

//HP
int HPValue_ = 100;

//ç≈ëÂHP	
int MaxHPValue_ = 100;

//å≥ÇÃÉXÉsÅ[Éh
float WalkSpeed_ = 0.15f;

//ÉGÉlÉãÉMÅ[ó 
float EnergyValue_ = 100.0f;

int BB = 10;

int BMB = 7;

int BAB = 3;

void PlayerState::initialize_state_() {

	//çUåÇóÕ
	AttackValue_ = 20;
	//ñhå‰óÕ
	DefenseValue_ = 20;
	//HP
	HPValue_ = 100;
	//ç≈ëÂHP	
	MaxHPValue_ = 100;
	//ï‡çsë¨ìx
	WalkSpeed_ = 0.15f;

	EnergyValue_ = 100.0f;

}

int PlayerState::Attack() const {

	return AttackValue_;
}

void PlayerState::AddAttack(int changeAT) {


	AttackValue_ += changeAT;

}

int PlayerState::Defense() const {

	return DefenseValue_;
}

void PlayerState::AddDefense(int changeDE) {


	DefenseValue_ += changeDE;

}

int PlayerState::HP() const {

	return HPValue_;
}

void PlayerState::AddHP(int changeHP) {


	HPValue_ += changeHP;

}

int PlayerState::MaxHP() const {

	return MaxHPValue_;
}

void PlayerState::AddMaxHP(int changeMaxHP) {


	MaxHPValue_ += changeMaxHP;

}

float PlayerState::MoveSpeed() const {

	return WalkSpeed_;
}

void PlayerState::AddMoveS(float changeMS) {

	float ms = changeMS / 100.0f + 1.0f;

	WalkSpeed_ *= ms;

}

float PlayerState::Enargy() const {

	return EnergyValue_;
}

void PlayerState::AddEnargy(float changeE) {


	EnergyValue_ += changeE;

}

PlayerState::GunState PlayerState::gunstate_() {

	return GS;
}

void PlayerState::SetGunState(GunState gunstate) {

	GS = gunstate;

}

int PlayerState::BeamBullet() {

	return BB;
}

void PlayerState::SetBeamBullet(int Bullet) {

	BB = Bullet;

}

int PlayerState::BeamMagnumBullet() {

	return BMB;
}

void PlayerState::SetBeamMagnumBullet(int Bullet) {

	BMB = Bullet;

}

int PlayerState::BazookaBullet() {

	return BAB;
}

void PlayerState::SetBazookaBullet(int Bullet) {

	BAB = Bullet;

}
