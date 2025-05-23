#include "Player/Player.h"
#include "PlayerState.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Gun/GunControl.h"
#include "PlayerBullet/AttackRange.h"
#include "Common/GameData.h"
#include "AllRangeUnits/ControlUnits.h"
#include "GSeffect.h"
#include "imgui/imgui.h"
#include "Scene/Screen.h"
#define GS_ENABLE_DITHER_TRANSPARENCY
#include <GSstandard_shader.h> 

//モーション番号
enum {
	//アイドルモーション
	Motion_Idle_G = 0,
	Motion_Idle_A = 1,
	//移動
	Motion_WarkF_G = 2,
	Motion_WarkB_G = 3,
	Motion_WarkL_G = 4,
	Motion_WarkR_G = 5,
	//空中移動
	Motion_WarkF_A = 6,
	Motion_WarkB_A = 7,
	Motion_WarkL_A = 8,
	Motion_WarkR_A = 9,
	//移動攻撃
	Motion_MAttackF_G = 10,
	Motion_MAttackB_G = 11,
	Motion_MAttackL_G = 12,
	Motion_MAttackR_G = 13,
	//空中移動攻撃
	Motion_MAttackF_A = 14,
	Motion_MAttackB_A = 15,
	Motion_MAttackL_A = 16,
	Motion_MAttackR_A = 17,
	//走り
	Motion_RunF_GunEarth = 18,
	Motion_RunB_GunEarth = 19,
	Motion_RunL_GunEarth = 20,
	Motion_RunR_GunEarth = 21,
	//空中高速移動
	Motion_RunF_GunAir = 22,
	Motion_RunL_GunAir = 23,
	Motion_RunR_GunAir = 24,
	//その場での攻撃
	Motion_Attack_GunEarth = 25,
	//その場での攻撃(空中)
	Motion_Attack1_GunAir = 26,
	//ジャンプ
	Motion_JumpStart_GunEarth = 27,
	Motion_Jump_GunEarth = 28,
	Motion_JumpEnd_GunEarth = 29,
	//着地
	Motion_Landing_GunEarth = 30,
	//地上でダメージを受けたとき
	Motion_Damage_GunEarth = 31,
	//空中でダメージを受けたとき
	Motion_Damage_GunAir = 32,
};

//自分の高さ
const float PlayerHeight{ 1.f };
//衝突判定用の半径
const float PlayerRadius{ 0.5f };
//足元のオフセット
const float footOffset_{ 0.1f };
//重力値
const float Gravity{ -0.016f };

//移動状態に応じて速度の倍率を変更
const float groundRunSpeed_{ 1.6f };
const float skyMoveSpeed_{ 1.3f };
const float skyRunSpeed_{ 2.0f };

//ジャンプ時の高さ
const float JumpHight{ 0.3f };

//コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player,Mesh_Player,Mesh_Player,Motion_Idle_G,true },
	motion_{ Motion_Idle_G },
	motion_loop_{ true },
	state_{ State::Move },
	state_timer_{ 0.f },
	CanBullet{ 40 },
	cameraSensitivity_{ 2.0f },
	vernierState_{ VernierState::down },
	explosionTimer_{ 180.0f }
{
	gsInitDefaultShader();

	//ワールド設定
	world_ = world;
	// タグ名の設定
	tag_ = "PlayerTag";
	// 名前の設定
	name_ = "Player";
	//衝突判定球の設定
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0.f,PlayerHeight,0.f} };
	//座標の初期化
	transform_.position(position);

	//初期の回転角度の調整
	transform_.rotate(0, -92, 0);

	//メッシュの変換行列を初期化
	mesh_.Transform(transform_.localToWorldMatrix());

	//プレイヤーのステータス生成
	playerstate_ = new PlayerState();

	//プレイヤーのステータス初期化
	playerstate_->initialize_state_();

	//アニメーション中のイベント設定
	SetAnimationEvent();

	gsSetVolumeSE(SE_BeamLifle, 0.8f);
	gsSetVolumeSE(SE_BeamMagnum, 0.5f);
	gsSetVolumeSE(SE_Bazooca, 1.0f);

	//無敵フラグ
	collisionInvalid_ = true;
}

//デストラクタ
Player::~Player() {
	//プレイヤーステータス削除
	delete playerstate_;
}

//更新
void Player::update(float delta_time) {
	//自身の座標
	myPos_ = transform_.position();

	//自身の移動速度
	walkSpeed_ = playerstate_->moveSpeed();

	//体力が一定値以上かどうか
	if (playerState_()->hp() >= playerState_()->maxHP() * 0.3f) hpReductionFrag_ = false;
	else hpReductionFrag_ = true;

	if (world_->gameData()->playerSupply()) {
		//ワールド変換行列を設定
		mesh_.Transform(transform_.localToWorldMatrix());
		return;
	}

	//EXスキル発動
	if (state_ != State::Damage && state_ != State::Die) {
		if (gsGetKeyTrigger(GKEY_Q) && playerState_()->exSkillPoint() >= 100 && !exSkillFinish_) {
			exSkill_ = exSkillRrocess_ = exSkillFinish_ = true;
			//バフのエフェクト
			exBuffEffect_ = gsPlayEffect(Effect_EXBuff, &myPos_);
			auraEffect_ = gsPlayEffect(Effect_aura, &myPos_);
		}
	}

	//無敵時間
	if (damageFrag_) {
		invincibleTimer_ -= delta_time;
		if (invincibleTimer_ <= 0) {
			invincibleTimer_ = assignmnetInvincibleTimer_;
			damageFrag_ = false;
			meshAlpha_ = 1.0f;
		}
	}

	//EXスキル処理
	if (exSkill_)exSkill(delta_time);

	//状態の更新
	update_state(delta_time);

	//マウスで左右方向で方向を変える
	int mx, my, mz;
	gsGetMouseVelocity(&mx, &my, &mz);
	cameraYaw_ = (float)-mx * 0.1f * cameraSensitivity_;
	transform_.rotate(0.0f, cameraYaw_ * delta_time, 0.0f);

	//飛んでいるか
	if (isFly_) {
		Fly(delta_time);
	}
	else {
		//エネルギーチャージ
		if (playerstate_->enargy() < 100) {
			playerstate_->addEnargy(delta_time * 0.5f);
		}
		//バーニアエフェクトの停止
		gsStopEffect(vernierEffectSS1_);
		gsStopEffect(vernierEffectSS2_);
		gsStopEffect(vernierEffectS1_);
		gsStopEffect(vernierEffectS2_);
		gsStopEffect(vernierEffectL1_);
		gsStopEffect(vernierEffectL2_);
		//重力値を更新
		velocity_.y += Gravity * delta_time;
		//重力を加える
		transform_.translate(0.f, velocity_.y, 0.f);
	}

	//フィールドとの衝突判定
	collide_field();
	//モーションを変更
	mesh_.ChangeMotion(motion_, motion_loop_);
	//メッシュのモーションを更新
	mesh_.Update(delta_time);
	//ワールド変換行列を設定
	mesh_.Transform(transform_.localToWorldMatrix());

	isJumpTime_ -= delta_time;
	if (isJumpTime_ < 0.0f) {
		isJump_ = true;
	}

	//装備している武器に応じてUIのサイズを変える
	weaponSilhouetteSize();

	//エフェクトの位置などの更新
	effectUpdate(delta_time);

	if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_2)) {
		playerstate_->setExSkillPoint(100);
	}
}

void Player::effectUpdate(float delta_time) {

	switch (vernierState_)
	{
	case Player::VernierState::up:

		localMatrix_ = GSmatrix4::TRS(vernierEffectL1Pos_, GSquaternion::euler(vernierEffect1Euler_), vernierEffectLScale_);
		effectWorld_ = localMatrix_ * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(vernierEffectL1_, &effectWorld_);

		localMatrix_ = GSmatrix4::TRS(vernierEffectL2Pos_, GSquaternion::euler(vernierEffect2Euler_), vernierEffectLScale_);
		effectWorld_ = localMatrix_ * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(vernierEffectL2_, &effectWorld_);
		break;
	case Player::VernierState::hover:
		localMatrix_ = GSmatrix4::TRS(vernierEffectS1Pos_, GSquaternion::euler(vernierEffect1Euler_), vernierEffectSScale_);
		effectWorld_ = localMatrix_ * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(vernierEffectS1_, &effectWorld_);

		localMatrix_ = GSmatrix4::TRS(vernierEffectS2Pos_, GSquaternion::euler(vernierEffect2Euler_), vernierEffectSScale_);
		effectWorld_ = localMatrix_ * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(vernierEffectS2_, &effectWorld_);
		break;
	case Player::VernierState::down:
		localMatrix_ = GSmatrix4::TRS(vernierEffectSS1Pos_, GSquaternion::euler(vernierEffect1Euler_), vernierEffectSSScale_);
		effectWorld_ = localMatrix_ * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(vernierEffectSS1_, &effectWorld_);

		localMatrix_ = GSmatrix4::TRS(vernierEffectSS2Pos_, GSquaternion::euler(vernierEffect2Euler_), vernierEffectSSScale_);
		effectWorld_ = localMatrix_ * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(vernierEffectSS2_, &effectWorld_);
		break;
	}

	//一定値以下かつ死亡処理に入っていない場合
	if (hpReductionFrag_ && state_ != Player::State::Die) {

		dastMakeTimer_ -= delta_time;

		if (dastMakeTimer_ <= 0) {

			gsStopEffect(dustEffect_);

			dastMakePos_ = GSvector3{ (float)gsRandf(-0.5,0.5),(float)gsRandf(-1,1) ,(float)gsRandf(-0.5,0.5) } + myPos_;

			dastMakePos_.y += PlayerHeight;

			dustEffect_ = gsPlayEffect(Effect_FootDust, &dastMakePos_);
			gsSetEffectColor(dustEffect_, &dustColor_);
			dastMakeTimer_ = 30.0f;
		}
	}

	if (exSkill_) {
		//EXバフのエフェクト
		localMatrix_ = GSmatrix4::TRS(exEffectPos_, GSquaternion::euler(exEffectEuler_), exEffectScale_);
		effectWorld_ = localMatrix_ * transform_.localToWorldMatrix();
		gsSetEffectMatrix(exBuffEffect_, &effectWorld_);
		gsSetEffectMatrix(auraEffect_, &effectWorld_);
	}

	//飛んでいないかつ移動状態にあるとき砂埃を発生させる
	if (!isFly_ && (velocity_.x != 0.0f || velocity_.z != 0.0f)) {

		footDastMakeTimer_ -= delta_time;

		if (footDastMakeTimer_ <= 0) {
			//足元に砂埃エフェクト生成
			footDustEffect_ = gsPlayEffect(Effect_FootDust, &myPos_);
			gsSetEffectColor(footDustEffect_, &footDustColor_);
			//生成クールタイム
			footDastMakeTimer_ = 30.0f;
		}
	}
}

//描画
void Player::draw()const {

	gsTextPos(100, 500);
	gsDrawText("soundValue_ %d", playerstate_->beamMagnumBullet());

	if (!notDrawMesh_) {

		if (exSkill_) {

			float transparency = gsGetDitheredTransparency();
			GScolor current_color;
			glGetFloatv(GL_CURRENT_COLOR, current_color);
			gsSetDitheredTransparency(meshAlpha_);
			glSecondaryColor3fv(exMeshColor_);
			mesh_.Draw();
			draw_weapon();
			gsSetDitheredTransparency(transparency);
			glColor4fv(current_color);
			glSecondaryColor3fv(nomalMeshColor_);
		}
		else {

			float transparency = gsGetDitheredTransparency();
			GScolor current_color;
			glGetFloatv(GL_CURRENT_COLOR, current_color);
			GScolor current_secondary_color;
			glGetFloatv(GL_CURRENT_SECONDARY_COLOR, current_secondary_color);
			gsSetDitheredTransparency(meshAlpha_);
			//メッシュの描画
			mesh_.Draw();
			//武器を描画
			draw_weapon();
			gsSetDitheredTransparency(transparency);
			glColor4fv(current_color);
			glSecondaryColor3fv(current_secondary_color);
		}
	}
}

//プレイヤーのUI描画
void Player::draw_gui() const {
	//各BERの描画
	drawHPBer();
	drawEXBer();
	drawThrusterBer();

	//武器のシルエットの描画
	drawWeaponSilhouette();

	reticle_position = { screenwidtht / 2, screenheight / 2 };
	//レティクルの描画
	gsDrawSprite2D(Texture_Reticle, &reticle_position, &reticle_rect, &reticle_center, NULL, NULL, 0.0f);
}

//HPバーの描画
void Player::drawHPBer()const {

	//HPバーのサイズ
	float maxhp = playerstate_->maxHP();
	float hp = playerstate_->hp();
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

//EXスキルバーの描画
void Player::drawEXBer()const {
	//必殺技のゲージ
	gsTextPos(180, 920);
	gsDrawText("必殺ゲージ:");
	int EXenargy = playerstate_->exSkillPoint();

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

	gsDrawSprite2D(Texture_EX, &exPosition_, &exRect_, NULL,
		&exColor_, &exScale_, 0.0f);

}

//武器のシルエットの描画
void Player::drawWeaponSilhouette()const {
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
	switch (playerstate_->gunstate_())
	{
	case PlayerState::GunState::Beamlifl:

		//弾残量
		asteriskPosition_ = { 1630,780 };
		gsDrawSprite2D(Texture_asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		asteriskPosition_ = { 1650,780 };
		gsDrawSprite2D(Texture_infinity, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);

		//マガジン残量
		asteriskPosition_ = { 1730,780 };
		gsDrawSprite2D(Texture_asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		asteriskPosition_ = { 1750,780 };
		gsDrawSprite2D(Texture_infinity, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);

		break;
	case PlayerState::GunState::BeamMagnumBullet:

		asteriskPosition_ = { 1630,850 };
		gsDrawSprite2D(Texture_asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		numPos_ = { 1650,850 };
		bulletNum_ = numRect_[playerstate_->beamMagnumBullet()];
		gsDrawSprite2D(Texture_Number, &numPos_, &bulletNum_, NULL, &numColor_, &numScale_, 0.0f);

		asteriskPosition_ = { 1730,850 };
		gsDrawSprite2D(Texture_asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		numPos_ = { 1750,850 };
		bulletNum_ = numRect_[playerstate_->beamMagnamMagazin()];
		gsDrawSprite2D(Texture_Number, &numPos_, &bulletNum_, NULL, &numColor_, &numScale_, 0.0f);
		break;
	case PlayerState::GunState::BazookaBullet:


		asteriskPosition_ = { 1630,930 };
		gsDrawSprite2D(Texture_asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		numPos_ = { 1650,930 };
		bulletNum_ = numRect_[playerstate_->bazookaBullet()];
		gsDrawSprite2D(Texture_Number, &numPos_, &bulletNum_, NULL, &numColor_, &numScale_, 0.0f);

		asteriskPosition_ = { 1730,930 };
		gsDrawSprite2D(Texture_asterisk, &asteriskPosition_, &asteriskRect_, NULL, &magajinColor_, &asteriskScale_, 0.0f);
		numPos_ = { 1750,930 };
		bulletNum_ = numRect_[playerstate_->bazookaMagazin()];
		gsDrawSprite2D(Texture_Number, &numPos_, &bulletNum_, NULL, &numColor_, &numScale_, 0.0f);
		break;
	}
}

//装備している向きに応じてUIのサイズとα値を変える
void Player::weaponSilhouetteSize() {
	//装備している銃に応じてUIのα値を変える
	switch (playerstate_->gunstate_())
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

//スラスター残量バーの描画
void Player::drawThrusterBer() const {
	//スラスター残量
	if (playerstate_->enargy() < 100) {

		gsDrawSprite2D(Texture_Buster2, &thrusterPosition_, &thrusterRect_, NULL,
			&thrusterColor_, &thrusterScale_, 0.0f);

		thrusterBackScale_.x = (playerstate_->MaxEnargy() - playerstate_->enargy()) / playerstate_->MaxEnargy();

		gsDrawSprite2D(Texture_Buster1, &thrusterBackPosition_, &thrusterBackRect_, NULL,
			&thrusterBackColor_, &thrusterBackScale_, 180.0f);
	}
}

//武器の描画
void Player::draw_weapon()const {

	//飛んでいるか
	if (isFly_) {
		//手の位置に銃を描画
		glPushMatrix();
		glMultMatrixf(mesh_.BoneMatrices(37));
		gsDrawMesh(Mesh_Weapon);
		glPopMatrix();
	}
	else if (!isFly_) {
		//手の位置に銃を描画
		glPushMatrix();
		glMultMatrixf(mesh_.BoneMatrices(36));
		gsDrawMesh(Mesh_Weapon);
		glPopMatrix();
	}
}

//衝突リアクション
void Player::react(Actor& other) {

	//すでにダメージ状態にあるとき　HPが０になったとき　補給時はダメージ受けないようにする
	if (state_ == State::Damage || playerstate_->hp() <= 0 || world_->gameData()->playerSupply())return;

	if (other.tag() == "EnemyBulletTag" && !collisionInvalid_ && !damageFrag_) {
		int damage = static_cast<BasicAttackCollider*>(&other)->GetAttackValue();

		//ダメージ処理
		playerstate_->AddHP(-damage);

		if (playerstate_->hp() <= 0) {

			//バーニア停止
			gsStopEffect(vernierEffectL1_);
			gsStopEffect(vernierEffectL2_);
			gsStopEffect(vernierEffectS1_);
			gsStopEffect(vernierEffectS2_);
			gsStopEffect(vernierEffectSS1_);
			gsStopEffect(vernierEffectSS2_);

			//必殺時に出るエフェクト
			gsStopEffect(exBuffEffect_);
			gsStopEffect(auraEffect_);

			explosionEffect_ = gsPlayEffect(Effect_ExplosionL, &myPos_);
			notDrawMesh_ = true;
			state_ = Player::State::Die;
			return;
		}
		else {

			//ターゲット方向のベクトルを求める
			GSvector3 to_target = other.transform().position() - myPos_;
			//ｙ成分は無効にする
			to_target.y = 0.f;
			//ターゲット方向と逆方向にノックバックする移動量を求める
			if (other.name() == "EnemyDamageRange") {
				velocity_ = -to_target.getNormalized();
			}
			else {
				velocity_ = -to_target.getNormalized() * 0.4f;
			}

			//ダメージ状態に遷移
			hitEffect_ = gsPlayEffect(Effect_Hit, &myPos_);
			damageFrag_ = true;
			meshAlpha_ = 0.5f;
			state_ = Player::State::Damage;
			return;
		}
	}
	//敵と衝突したか？
	if (other.tag() == "EnemyTag")collide_actor(other);
}

PlayerState* Player::playerState_() const {
	return playerstate_;
}

//状態の更新
void Player::update_state(float delta_time) {
	//状態遷移
	switch (state_)
	{
	case Player::State::Move:
		move(delta_time);
		break;
	case Player::State::ShootAttack:
		shoot(delta_time);
		break;
	case Player::State::Damage:
		damage(delta_time);
		break;
	case Player::State::Die:
		dieProcess(delta_time);
		break;
	case Player::State::JumpStart:
		jump_start(delta_time);
		break;
	case Player::State::Jump:
		jump_(delta_time);
		break;
	case Player::State::JumpEnd:
		jump_end(delta_time);
		break;
	case Player::State::MoveShootAttack:
		move_attack(delta_time);
		break;
	}
	//状態タイマの更新
	state_timer_ += delta_time;
}

//状態の変更
void Player::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.f;
}

//移動処理
void Player::move(float delta_time) {

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };

	//銃撃
	if (gsGetMouseButtonState(GMOUSE_BUTTON_1) && AttackJudgment()) {
		ChangeFire();
		return;
	}

	GSvector3 velocity{ 0.f,0.f,0.f };
	velocity = velocity.normalized() * walkSpeed_ * delta_time;

	GSint motion;

	if (isFly_) 	motion = Motion_Idle_A;
	else motion = Motion_Idle_G;

	//移動しているか？
	if (velocity.length() != 0.f) {
		//向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity), 12.0f * delta_time);
		transform_.rotation(rotation);

		//移動中のモーションにする
		if (isFly_) motion = Motion_WarkF_A;
		else motion = Motion_WarkF_G;
	}
	//モーションの変更
	change_state(State::Move, motion);

	if (gsGetKeyState(GKEY_W)) {
		if (gsGetKeyState(GKEY_LSHIFT)) {

			if (isFly_) {
				motion = Motion_RunF_GunAir;
				forward_speed = walkSpeed_ * skyRunSpeed_;
			}
			else {
				motion = Motion_RunF_GunEarth;
				forward_speed = walkSpeed_ * groundRunSpeed_;
			}
		}
		else {
			if (isFly_) {
				motion = Motion_WarkF_A;
				forward_speed = walkSpeed_ * skyMoveSpeed_;
			}
			else {
				motion = Motion_WarkF_G;
				forward_speed = walkSpeed_;
			}
		}
	}
	if (gsGetKeyState(GKEY_S)) {
		if (gsGetKeyState(GKEY_LSHIFT) && !isFly_) {

			forward_speed = -walkSpeed_ * groundRunSpeed_;
			if (!isFly_) motion = Motion_RunB_GunEarth;

		}
		else {
			if (isFly_) {
				motion = Motion_WarkB_A;
				forward_speed = -walkSpeed_ * skyMoveSpeed_;
			}
			else {
				motion = Motion_WarkB_G;
				forward_speed = -walkSpeed_;
			}
		}
	}
	if (gsGetKeyState(GKEY_A)) {
		if (gsGetKeyState(GKEY_LSHIFT)) {

			if (isFly_) {
				motion = Motion_RunL_GunAir;
				side_speed = walkSpeed_ * skyRunSpeed_;
			}
			else {
				motion = Motion_RunL_GunEarth;
				side_speed = walkSpeed_ * groundRunSpeed_;
			}
		}
		else {
			if (isFly_) {
				motion = Motion_WarkL_A;
				side_speed = walkSpeed_ * skyMoveSpeed_;
			}
			else {
				motion = Motion_WarkL_G;
				side_speed = walkSpeed_;
			}
		}
	}
	if (gsGetKeyState(GKEY_D)) {
		if (gsGetKeyState(GKEY_LSHIFT)) {
			if (isFly_) {
				motion = Motion_RunR_GunAir;
				side_speed = -walkSpeed_ * skyRunSpeed_;
			}
			else {
				motion = Motion_RunR_GunEarth;
				side_speed = -walkSpeed_ * groundRunSpeed_;
			}
		}
		else {
			if (isFly_) {
				motion = Motion_WarkR_A;
				side_speed = -walkSpeed_ * skyMoveSpeed_;
			}
			else {
				motion = Motion_WarkR_G;
				side_speed = -walkSpeed_;
			}
		}
	}
	//移動状態にする
	change_state(State::Move, motion);

	velocity_.x = side_speed;
	velocity_.z = forward_speed;


	//平行移動する
	transform_.translate(velocity_.x * delta_time, 0.f, velocity_.z * delta_time);

	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && isJump_ && !isFly_) {
		// ジャンプ開始状態へ
		change_state(State::JumpStart, Motion_JumpStart_GunEarth);
		// ジャンプ
		velocity_.y = JumpHight;
		return;
	}
	ClampPos();
}

//弾が撃てるか
bool Player::AttackJudgment()const {

	//各種弾があるか
	return(playerState_()->gunstate_() == PlayerState::GunState::Beamlifl
		&& playerState_()->beamBullet() > 0 ||
		playerState_()->gunstate_() == PlayerState::GunState::BeamMagnumBullet
		&& playerState_()->beamMagnumBullet() > 0 || playerState_()->gunstate_() == PlayerState::GunState::BazookaBullet
		&& playerState_()->bazookaBullet() > 0);
}

void Player::ChangeFire() {

	if (isFly_) {

		if (velocity_.x == 0.0f && velocity_.z == 0.0f) {
			//射撃ステータスに移行
			change_state(State::ShootAttack, Motion_Attack1_GunAir);
			//攻撃可能フラグをオン
			isAttack_ = true;
		}

		//移動ボタンが押されたら移動中の攻撃にステータスを変える //仮で地上の時と同じようにしてみる
		if (gsGetKeyState(GKEY_W)) change_state(State::MoveShootAttack, Motion_MAttackF_A);
		if (gsGetKeyState(GKEY_S)) change_state(State::MoveShootAttack, Motion_MAttackB_A);
		if (gsGetKeyState(GKEY_A)) change_state(State::MoveShootAttack, Motion_MAttackL_A);
		if (gsGetKeyState(GKEY_D)) change_state(State::MoveShootAttack, Motion_MAttackR_A);
	}
	else if (!isFly_) {

		if (velocity_.x == 0.0f && velocity_.z == 0.0f) {
			//射撃ステータスに移行
			change_state(State::ShootAttack, Motion_Attack_GunEarth);
			//攻撃可能フラグをオン
			isAttack_ = true;
		}

		//移動ボタンが押されたら移動中の攻撃にステータスを変える
		if (gsGetKeyState(GKEY_W)) change_state(State::MoveShootAttack, Motion_MAttackF_G);
		if (gsGetKeyState(GKEY_S)) change_state(State::MoveShootAttack, Motion_MAttackB_G);
		if (gsGetKeyState(GKEY_A)) change_state(State::MoveShootAttack, Motion_MAttackL_G);
		if (gsGetKeyState(GKEY_D)) change_state(State::MoveShootAttack, Motion_MAttackR_G);
	}
}

//射撃
void Player::shoot(float delta_time) {

	//撃っている途中で０になったらステータス移行
	JudgementBullet();

	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && isJump_ && !isFly_) {
		change_state(State::JumpStart, Motion_JumpStart_GunEarth);
		velocity_.y = JumpHight;
	}

	//弾生成
	if (isAttack_) {
		isAttack_ = false;
		generate_bullet();
	}
	if (state_timer_ >= CanBullet)move(delta_time);
}

//撃っている最中に０になったらアイドル状態に遷移
void Player::JudgementBullet() {

	if (playerState_()->gunstate_() == PlayerState::GunState::Beamlifl
		&& playerState_()->beamBullet() <= 0 ||
		playerState_()->gunstate_() == PlayerState::GunState::BeamMagnumBullet
		&& playerState_()->beamMagnumBullet() <= 0 ||
		playerState_()->gunstate_() == PlayerState::GunState::BazookaBullet
		&& playerState_()->bazookaBullet() <= 0) {

		if (isFly_)change_state(State::Move, Motion_Idle_A);
		else if (!isFly_)change_state(State::Move, Motion_Idle_G);
	}
}

//ダメージ中
void Player::damage(float delta_time) {
	//ノックバックする（ワールド座標系）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//減速させる
	velocity_ -= GSvector3{ velocity_.x,0.f,velocity_.z }*0.5f * delta_time;

	//移動状態に戻す
	state_ = Player::State::Move;
}

void Player::dieProcess(float delta_time) {

	//エフェクト再生時間
	explosionTimer_ -= delta_time;

	//エフェクト再生終了したら
	if (explosionTimer_ < 0 && !dieFrag_) {

		gsStopEffect(explosionEffect_);

		//死亡フラグを上げる
		dieFrag_ = true;

		//ゲームにプレイヤーの死亡を知らせる
		world_->gameData()->setPlayerDie(true);
	}
}

//ジャンプ開始
void Player::jump_start(float delta_time) {

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };
	//WASD移動
	if (gsGetKeyState(GKEY_W)) {
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = walkSpeed_ * groundRunSpeed_;
		else forward_speed = walkSpeed_;
	}
	if (gsGetKeyState(GKEY_S)) {
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = -walkSpeed_ * groundRunSpeed_;
		else forward_speed = -walkSpeed_;
	}
	if (gsGetKeyState(GKEY_A)) {
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = walkSpeed_ * groundRunSpeed_;
		else side_speed = walkSpeed_;
	}
	if (gsGetKeyState(GKEY_D)) {
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = -walkSpeed_ * groundRunSpeed_;
		else side_speed = -walkSpeed_;
	}

	velocity_.x = side_speed;
	velocity_.z = forward_speed;
	transform_.translate(velocity_.x * delta_time, 0.f, velocity_.z * delta_time);

	if (gsGetKeyTrigger(GKEY_SPACE)) {

		velocity_.y = 0.0f;
		isFly_ = true;
		change_state(State::JumpEnd, Motion_JumpEnd_GunEarth);
	}

	if (state_timer_ >= mesh_.MotionEndTime()) {
		// ある程度したら、すぐにジャンプ中モーションへ
		change_state(State::JumpEnd, Motion_Jump_GunEarth);
	}
	ClampPos();
}

//ジャンプ中
void Player::jump_(float delta_time) {

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };
	//WASD移動
	if (gsGetKeyState(GKEY_W)) {
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = walkSpeed_ * groundRunSpeed_;
		else forward_speed = walkSpeed_;
	}
	if (gsGetKeyState(GKEY_S)) {
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = -walkSpeed_ * groundRunSpeed_;
		else forward_speed = -walkSpeed_;
	}
	if (gsGetKeyState(GKEY_A)) {
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = walkSpeed_ * groundRunSpeed_;
		else side_speed = walkSpeed_;
	}
	if (gsGetKeyState(GKEY_D)) {
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = -walkSpeed_ * groundRunSpeed_;
		else side_speed = -walkSpeed_;
	}

	velocity_.x = side_speed;
	velocity_.z = forward_speed;
	transform_.translate(velocity_.x * delta_time, 0.f, velocity_.z * delta_time);

	if (gsGetKeyTrigger(GKEY_SPACE)) {
		velocity_.y = 0.0f;
		isFly_ = true;

		change_state(State::JumpEnd, Motion_JumpEnd_GunEarth);
	}
	ClampPos();
}

//ジャンプ終了
void Player::jump_end(float delta_time) {
	if (state_timer_ >= 3) {

		change_state(State::Move, Motion_Idle_G);

		isJump_ = false;
		isJumpTime_ = 15.0f;
	}
	ClampPos();
}

//移動中の射撃
void Player::move_attack(float delta_time) {

	//撃っている途中で０になったらステータス移行
	JudgementBullet();

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };

	//WASD移動
	if (gsGetKeyState(GKEY_W)) {
		forward_speed = walkSpeed_;

		if (isFly_)motion_ = Motion_MAttackF_A;
		else if (!isFly_)motion_ = Motion_MAttackF_G;
	}
	if (gsGetKeyState(GKEY_S)) {
		forward_speed = -walkSpeed_;

		if (isFly_)motion_ = Motion_MAttackB_A;
		else if (!isFly_)motion_ = Motion_MAttackB_G;
	}
	if (gsGetKeyState(GKEY_A)) {
		side_speed = walkSpeed_;

		if (isFly_)motion_ = Motion_MAttackL_A;
		else if (!isFly_)motion_ = Motion_MAttackL_G;
	}
	if (gsGetKeyState(GKEY_D)) {
		side_speed = -walkSpeed_;

		if (isFly_)motion_ = Motion_MAttackR_A;
		else if (!isFly_)motion_ = Motion_MAttackR_G;
	}

	velocity_.x = side_speed;
	velocity_.z = forward_speed;

	transform_.translate(velocity_.x * delta_time, 0.f, velocity_.z * delta_time);

	//立ち止まったら攻撃開始状態へ
	if (velocity_.x == 0.0f && velocity_.z == 0.0f) {
		if (isFly_)change_state(State::ShootAttack, Motion_Attack1_GunAir);
		else if (!isFly_)change_state(State::ShootAttack, Motion_Attack_GunEarth);
	}

	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && isJump_ && !isFly_) {
		// ジャンプ開始状態へ
		change_state(State::JumpStart, Motion_JumpStart_GunEarth, false);
		velocity_.y = JumpHight;
	}

	//ある程度立ったら移動状態医へ
	if (state_timer_ >= mesh_.MotionEndTime()) move(delta_time);
	ClampPos();
}

//飛行
void Player::Fly(float delta_time) {

	//エネルギー減少
	//playerstate_->addEnargy(-delta_time * 0.1f);

	float UpSpeed{ 0.0f };

	if (gsGetKeyState(GKEY_SPACE) && transform_.position().y < 0.0) {
		UpSpeed += walkSpeed_;
		vernierState_ = Player::VernierState::up;
	}
	else if (gsGetKeyState(GKEY_LCONTROL)) {
		UpSpeed -= walkSpeed_;
		vernierState_ = Player::VernierState::down;
	}
	else {
		vernierState_ = Player::VernierState::hover;
	}

	//エフェクト再生
	if (comparisonVernierstate_ != vernierState_) {
		switch (vernierState_)
		{
		case Player::VernierState::up:
			gsStopEffect(vernierEffectS1_);
			gsStopEffect(vernierEffectS2_);
			gsStopEffect(vernierEffectSS1_);
			gsStopEffect(vernierEffectSS2_);
			vernierEffectL1_ = gsPlayEffect(Effect_VernierBL, &myPos_);
			vernierEffectL2_ = gsPlayEffect(Effect_VernierBL, &myPos_);
			break;
		case Player::VernierState::hover:
			gsStopEffect(vernierEffectL1_);
			gsStopEffect(vernierEffectL2_);
			gsStopEffect(vernierEffectSS1_);
			gsStopEffect(vernierEffectSS2_);
			vernierEffectS1_ = gsPlayEffect(Effect_VernierBS, &myPos_);
			vernierEffectS2_ = gsPlayEffect(Effect_VernierBS, &myPos_);
			break;
		case Player::VernierState::down:
			gsStopEffect(vernierEffectL1_);
			gsStopEffect(vernierEffectL2_);
			gsStopEffect(vernierEffectS1_);
			gsStopEffect(vernierEffectS2_);
			vernierEffectSS1_ = gsPlayEffect(Effect_VernierBSS, &myPos_);
			vernierEffectSS2_ = gsPlayEffect(Effect_VernierBSS, &myPos_);
			break;
		}
		comparisonVernierstate_ = vernierState_;
	}

	velocity_.y = UpSpeed;

	transform_.translate(0, velocity_.y * delta_time, 0);

	if (playerstate_->enargy() <= 0.0f)isFly_ = false;
}

void Player::exSkill(float delta_time) {

	if (exSkillRrocess_) {

		int expoint = playerState_()->exSkillPoint();
		if (expoint >= 100 && expoint < 200) {

			//プレイヤーのステータス上昇
			playerstate_->setEXSkill(1.2f);
			playerState_()->setExSkillPoint(-100);
		}
		else if (expoint >= 200 && expoint < 300) {

			//プレイヤーのステータス上昇
			playerstate_->setEXSkill(1.5f);
			//ファンネル生成
			MakeUnit();
			playerState_()->setExSkillPoint(-200);
		}
		else {

			//プレイヤーのステータス上昇
			playerstate_->setEXSkill(2.0f);
			//ファンネル生成
			MakeUnit();
			//無敵
			collisionInvalid_ = true;
			playerState_()->setExSkillPoint(-300);
		}
		exSkillRrocess_ = false;
	}

	//継続時間
	exSkillTimer_ -= delta_time;

	if (exSkillTimer_ <= 0) {

		gsStopEffect(exBuffEffect_);
		gsStopEffect(auraEffect_);

		//プレイヤーのステータスを発動前に戻す
		playerState_()->resetEXSkill();

		//ファンネル撤退
		if (units_ != NULL)units_->changeFrag(true);
		//無敵解除
		collisionInvalid_ = false;

		//初期化
		exSkill_ = exSkillFinish_ = false;
		exSkillTimer_ = assignmentExSkillTimer_;
	}
}

void Player::MakeUnit() {
	GSvector3 makepos = myPos_;
	//生成位置の調整
	makepos.y += 1.0f;
	makepos -= transform_.localToWorldMatrix().forward() * 0.5;
	units_ = new ControlUnits{ world_,makepos };
	world_->add_actor(units_);
}

//フィールドとの衝突判定
void Player::collide_field() {
	//壁との衝突判定（球体との判定）
	GSvector3 center;//衝突後の球体の中心位置
	if (world_->field()->collide(collider(), &center)) {
		//ｙ座標は変更しない
		center.y = transform_.position().y;
		//補正後の座標に変更する
		transform_.position(center);
	}
	//地面との衝突判定（線分との交点）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.f,-footOffset_,0.f };
	GSvector3 intersect;//地面との交点
	if (world_->field()->collide(line, &intersect)) {
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.f;

		if (state_ == State::Jump) {
			// 速度を止める
			velocity_ = GSvector3::zero();

			change_state(State::JumpEnd, Motion_JumpEnd_GunEarth);
		}
		if (isFly_)isFly_ = false;
	}
}

//アクターとの衝突処理
void Player::collide_actor(Actor& other) {
	//y座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.y = 0.f;
	GSvector3 target = other.transform().position();
	target.y = 0.f;
	//相手との距離
	float distance = GSvector3::distance(position, target);
	//衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius + other.collider().radius;
	//衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	//重なっている部分の半分の距離だけ離れる移動値を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	collide_field();
}

//弾の生成
void Player::generate_bullet() {

	//ガンコントローラーを取得
	gc_ = static_cast<GunControl*>(world_->find_actor("GunControl"));
	gc_->Fire();
}

//モーション中に弾を生成する
void Player::can_bullet() {

	//マウスクリックで射撃
	if (gsGetMouseButtonState(GMOUSE_BUTTON_1)) {
		generate_bullet();
	}
}

//アニメーションイベントの設定
void Player::SetAnimationEvent() {

	mesh_.AddEvent(Motion_MAttackF_G, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_G, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_G, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_G, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_G, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_G, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_G, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_G, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_G, CanBullet * 8, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_G, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_G, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_G, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_G, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_G, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_G, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_G, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_G, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_G, CanBullet * 8, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_G, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_G, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_G, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_G, CanBullet * 3, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_G, CanBullet * 4, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_G, CanBullet * 5, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_G, CanBullet * 6, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_G, CanBullet * 7, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_G, CanBullet * 8, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_G, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_G, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_G, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_G, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_G, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_G, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_G, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_G, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_G, CanBullet * 8, [this] {can_bullet(); });

	mesh_.AddEvent(Motion_MAttackF_A, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_A, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_A, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_A, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_A, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_A, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_A, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_A, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_A, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_A, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_A, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_A, CanBullet * 3, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_A, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_A, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_A, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_A, CanBullet * 3, [this] {can_bullet(); });
}

void Player::ClampPos() {
	GSvector3 position = transform_.position();
	position.x = CLAMP(position.x, -88.0f, 210.0f);
	position.z = CLAMP(position.z, -21.0f, 38.0f);
	transform_.position(position);
}