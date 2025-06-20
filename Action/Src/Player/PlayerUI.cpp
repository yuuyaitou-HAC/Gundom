#include "Player/PlayerUI.h"
#include "Player/PlayerState.h"
#include "Common/Assets.h"
#include "Scene/Screen.h"

PlayerUI::PlayerUI(PlayerState* ps) {
	//プレイヤーステータス取得
	playerState_ = ps;
}

void PlayerUI::update(float delta_time) {
	weaponSilhouetteSize();
}

void PlayerUI::drawgui() const {
	draw_HPBer();
	draw_EXBer();
	draw_WeaponSilhouette();
	draw_ThrusterBer();
	draw_Reticle();
}

void PlayerUI::draw_HPBer() const {

	//HPバーのサイズ
	float maxhp = playerState_->maxHP();
	float hp = playerState_->hp();
	hpBarScale_ = (maxhp - hp) / maxhp;
	hpBarScale_ = CLAMP(hpBarScale_, 0, 1);

	//HPバー(青)
	gsDrawSprite2D(Texture_HPBer, &hpBerPosition_, &hpBerRect_,
		NULL, &hpBerColor_, &hpBerScale_, 0.0f);

	//HPバー(灰)
	GSvector2 HPBackScale{ hpBarScale_,1 };
	gsDrawSprite2D(Texture_HPBack, &hpBackPosition_, &hpBackRect_,
		NULL, &hpBackColor_, &HPBackScale, 180.0f);

	//HP表示
	gsDrawSprite2D(Texture_HP, &hpPosition_, &hpRect_,
		NULL, &hpColor_, &hpScale_, 0);
}

void PlayerUI::draw_EXBer() const {
	//必殺技のゲージ
	gsTextPos(180, 920);
	gsDrawText("必殺ゲージ:");
	int EXenargy = playerState_->exSkillPoint();

	if (EXenargy < 100) {
		//下地
		gsDrawSprite2D(Texture_EX1, &exBerPosition_, &exBerRect_, NULL, &exBerColor_, &exBerScale_, 0.0f);

		enargyBarScale_ = { (float)EXenargy / 100, 1.0 };
		//可動
		gsDrawSprite2D(Texture_EX2, &exBerPosition_, &exBerRect_, NULL, &exBerColor_, &enargyBarScale_, 0.0f);
	}
	else if (EXenargy >= 100 && EXenargy < 200) {
		//下地
		gsDrawSprite2D(Texture_EX2, &exBerPosition_, &exBerRect_, NULL, &exBerColor_, &exBerScale_, 0.0f);

		enargyBarScale_ = { ((float)EXenargy - 100) / 100, 1.0 };
		//可動
		gsDrawSprite2D(Texture_EX3, &exBerPosition_, &exBerRect_, NULL, &exBerColor_, &enargyBarScale_, 0.0f);

		gsDrawSprite2D(Texture_EX2Ball, &exBallPosition1_, &exBallRect_, NULL,
			&exBallColor_, &exBallScale_, 0.0f);
	}
	else if (EXenargy >= 200 && EXenargy < 300) {
		//下地
		gsDrawSprite2D(Texture_EX3, &exBerPosition_, &exBerRect_, NULL, &exBerColor_, &exBerScale_, 0.0f);
		enargyBarScale_ = { ((float)EXenargy - 200) / 100, 1.0 };
		//可動
		gsDrawSprite2D(Texture_EX4, &exBerPosition_, &exBerRect_, NULL, &exBerColor_, &enargyBarScale_, 0.0f);

		gsDrawSprite2D(Texture_EX2Ball, &exBallPosition1_, &exBallRect_, NULL,
			&exBallColor_, &exBallScale_, 0.0f);

		gsDrawSprite2D(Texture_EX3Ball, &exBallPosition2_, &exBallRect_, NULL,
			&exBallColor_, &exBallScale_, 0.0f);
	}
	else {
		//下地
		gsDrawSprite2D(Texture_EX4, &exBerPosition_, &exBerRect_, NULL, &exBerColor_, &exBerScale_, 0.0f);

		gsDrawSprite2D(Texture_EX2Ball, &exBallPosition1_, &exBallRect_, NULL,
			&exBallColor_, &exBallScale_, 0.0f);

		gsDrawSprite2D(Texture_EX3Ball, &exBallPosition2_, &exBallRect_, NULL,
			&exBallColor_, &exBallScale_, 0.0f);

		gsDrawSprite2D(Texture_EX4Ball, &exBallPosition3_, &exBallRect_, NULL,
			&exBallColor_, &exBallScale_, 0.0f);
	}

	gsDrawSprite2D(Texture_EX, &exPosition_, &exRect_, NULL, &exColor_, &exScale_, 0.0f);
}

void PlayerUI::draw_WeaponSilhouette() const {

	gsDrawSprite2D(Texture_BeamLifle, &beamLiflePosition_, &beamLifleRect_, NULL,
		&beamLifleColor_, &beamLifleScale_, 0.0f);

	gsDrawSprite2D(Texture_BeamMagnum, &beamMagnumPosition_, &beamMagnumRect_, NULL,
		&beamMagnumColor_, &beamMagnumScale_, 0.0f);

	gsDrawSprite2D(Texture_Bazooka, &bazookaPosition_, &bazookaRect_, NULL,
		&bazookaColor_, &bazookaScale_, 0.0f);

	//弾
	gsDrawSprite2D(Texture_Bullet, &bulletPosition_, &bulletRect_, NULL,
		&bulletColor_, &bulletScale_, 0.0f);

	//マガジン
	gsDrawSprite2D(Texture_Magajin, &magajinPosition_, &magajinRect_, NULL,
		&magajinColor_, &magajinScale_, 0.0f);

	//マガジン数や弾数表示
	switch (playerState_->gunstate_())
	{
	case PlayerState::GunState::Beamlifl:

		//弾残量
		asteriskPosition_ = { 1630,780 };
		gsDrawSprite2D(Texture_Asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		asteriskPosition_ = { 1650,780 };
		gsDrawSprite2D(Texture_Infinity, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);

		//マガジン残量
		asteriskPosition_ = { 1730,780 };
		gsDrawSprite2D(Texture_Asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		asteriskPosition_ = { 1750,780 };
		gsDrawSprite2D(Texture_Infinity, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);

		break;
	case PlayerState::GunState::BeamMagnumBullet:

		asteriskPosition_ = { 1630,850 };
		gsDrawSprite2D(Texture_Asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		numPos_ = { 1650,850 };
		bulletNum_ = numRect_[playerState_->beamMagnumBullet()];
		gsDrawSprite2D(Texture_Number, &numPos_, &bulletNum_, NULL, &numColor_, &numScale_, 0.0f);

		asteriskPosition_ = { 1730,850 };
		gsDrawSprite2D(Texture_Asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		numPos_ = { 1750,850 };
		bulletNum_ = numRect_[playerState_->beamMagnamMagazin()];
		gsDrawSprite2D(Texture_Number, &numPos_, &bulletNum_, NULL, &numColor_, &numScale_, 0.0f);
		break;
	case PlayerState::GunState::BazookaBullet:


		asteriskPosition_ = { 1630,930 };
		gsDrawSprite2D(Texture_Asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		numPos_ = { 1650,930 };
		bulletNum_ = numRect_[playerState_->bazookaBullet()];
		gsDrawSprite2D(Texture_Number, &numPos_, &bulletNum_, NULL, &numColor_, &numScale_, 0.0f);

		asteriskPosition_ = { 1730,930 };
		gsDrawSprite2D(Texture_Asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		numPos_ = { 1750,930 };
		bulletNum_ = numRect_[playerState_->bazookaMagazin()];
		gsDrawSprite2D(Texture_Number, &numPos_, &bulletNum_, NULL, &numColor_, &numScale_, 0.0f);
		break;
	}
}

void PlayerUI::weaponSilhouetteSize() {
	//装備している銃に応じてUIのα値を変える
	switch (playerState_->gunstate_())
	{
	case PlayerState::GunState::Beamlifl:
		beamLifleColor_.a = 1.0f;
		beamMagnumColor_.a = 0.5f;
		bazookaColor_.a = 0.5f;
		bulletPosition_.y = magajinPosition_.y = beamLiflePosition_.y;

		beamLifleScale_ = assignmentBeamLifleScale_ * magnification_;
		beamMagnumScale_ = assignmentBeamMagnumScale_;
		bazookaScale_ = assignmentBazookaScale_;

		break;
	case PlayerState::GunState::BeamMagnumBullet:
		beamLifleColor_.a = 0.5f;
		beamMagnumColor_.a = 1.0f;
		bazookaColor_.a = 0.5f;
		bulletPosition_.y = magajinPosition_.y = beamMagnumPosition_.y;

		beamLifleScale_ = assignmentBeamLifleScale_;
		beamMagnumScale_ = assignmentBeamMagnumScale_ * magnification_;
		bazookaScale_ = assignmentBazookaScale_;

		break;
	case PlayerState::GunState::BazookaBullet:
		beamLifleColor_.a = 0.5f;
		beamMagnumColor_.a = 0.5f;
		bazookaColor_.a = 1.0f;
		bulletPosition_.y = magajinPosition_.y = bazookaPosition_.y;

		beamLifleScale_ = assignmentBeamLifleScale_;
		beamMagnumScale_ = assignmentBeamMagnumScale_;
		bazookaScale_ = assignmentBazookaScale_ * magnification_;
		break;
	}
}

void PlayerUI::draw_ThrusterBer() const {
	//スラスター残量
	if (playerState_->enargy() < playerState_->MaxEnargy()) {

		gsDrawSprite2D(Texture_Buster2, &thrusterPosition_, &thrusterRect_, NULL,
			&thrusterColor_, &thrusterScale_, 0.0f);

		thrusterBackScale_.x = (playerState_->MaxEnargy() - playerState_->enargy()) / playerState_->MaxEnargy();

		gsDrawSprite2D(Texture_Buster1, &thrusterBackPosition_, &thrusterBackRect_, NULL,
			&thrusterBackColor_, &thrusterBackScale_, 180.0f);
	}
}

void PlayerUI::draw_Reticle() const {
	reticle_position = { screenwidtht / 2, screenheight / 2 };
	//レティクルの描画
	gsDrawSprite2D(Texture_Reticle, &reticle_position, &reticle_rect, &reticle_center, NULL, NULL, 0.0f);
}
