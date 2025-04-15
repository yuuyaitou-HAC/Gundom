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
#include <GSstandard_shader.h> 
#define GS_ENABLE_DITHER_TRANSPARENCY

//モーション番号
enum {
	//アイドルモーション
	Motion_Idle_GunEarth = 0,
	Motion_Idle_GunAir = 1,

	//移動
	Motion_WarkF_GunEarth = 3,
	Motion_WarkB_GunEarth = 4,
	Motion_WarkL_GunEarth = 5,
	Motion_WarkR_GunEarth = 6,

	//空中移動
	Motion_WarkF_GunAir = 7,
	Motion_WarkB_GunAir = 8,
	Motion_WarkL_GunAir = 9,
	Motion_WarkR_GunAir = 10,

	//移動攻撃
	Motion_MAttackF_GunEarth = 15,
	Motion_MAttackB_GunEarth = 16,
	Motion_MAttackL_GunEarth = 17,
	Motion_MAttackR_GunEarth = 18,

	//走り
	Motion_RunF_GunEarth = 19,
	Motion_RunB_GunEarth = 20,
	Motion_RunL_GunEarth = 21,
	Motion_RunR_GunEarth = 22,

	//空中高速移動
	Motion_RunF_GunAir = 23,
	Motion_RunL_GunAir = 24,
	Motion_RunR_GunAir = 25,

	//その場での攻撃
	Motion_Attack_GunEarth = 30,

	//その場での攻撃(空中)
	Motion_Attack1_GunAir = 31,
	Motion_Attack2_GunAir = 32,

	//ジャンプ
	Motion_JumpStart_GunEarth = 36,
	Motion_Jump_GunEarth = 37,
	Motion_JumpEnd_GunEarth = 38,

	//着地
	Motion_Landing_GunEarth = 40,

	//地上でダメージを受けたとき
	Motion_Damage_GunEarth = 45,

	//空中でダメージを受けたとき
	Motion_Damage_GunAir = 46,

	//死んだ
	Motion_Die_GunEarth = 49,

	//空中で死んだ
	Motion_Die_GunAir = 50,
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
	mesh_{ Mesh_Player,Mesh_Player,Mesh_Player,Motion_Idle_GunEarth,true },
	motion_{ Motion_Idle_GunEarth },
	motion_loop_{ true },
	state_{ State::Move },
	state_timer_{ 0.f },
	CanBullet{ 20 },
	CameraSensitivity{ 2.0f },
	vernierstate_{ VernierState::down },
	explosionTimer{ 180.0f }
{
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

	gsInitDefaultShader();

	//無敵フラグ
	collisionInvalid = true;
}

//デストラクタ
Player::~Player() {
	//プレイヤーステータス削除
	delete playerstate_;
}

//更新
void Player::update(float delta_time) {

	//自身の座標
	pos = transform_.position();

	//自身の移動速度
	walkSpeed = playerstate_->moveSpeed();

	//体力が一定値以上かどうか
	if (playerState_()->hp() >= playerState_()->maxHP() * 0.3f) HPReductionFrag = false;
	else HPReductionFrag = true;

	if (world_->gameData()->playerSupply()) {
		//ワールド変換行列を設定
		mesh_.Transform(transform_.localToWorldMatrix());
		return;
	}

	//EXスキル発動
	if (state_ != State::Damage && state_ != State::Die) {
		if (gsGetKeyTrigger(GKEY_Q) && playerState_()->exSkillPoint() >= 100 && !EXskillfinish_) {
			EXSkill_ = ExSkillRrocess = EXskillfinish_ = true;
			//バフのエフェクト
			effectExbuff = gsPlayEffect(Effect_EXBuff, &pos);
			effectaura = gsPlayEffect(Effect_aura, &pos);
		}
	}

	//無敵時間
	if (damageFrag_) {
		invincibleTimer_ -= delta_time;
		if (invincibleTimer_ <= 0) {
			invincibleTimer_ = assignmnetInvincibleTimer_;
			damageFrag_ = false;
			meshAlpha = 1.0f;
		}
	}

	//EXスキル処理
	if (EXSkill_)exSkill(delta_time);

	//状態の更新
	update_state(delta_time);

	//マウスで左右方向で方向を変える
	int mx, my, mz;
	gsGetMouseVelocity(&mx, &my, &mz);
	camerayaw_ = (float)-mx * 0.1f * CameraSensitivity;
	transform_.rotate(0.0f, camerayaw_ * delta_time, 0.0f);

	//飛んでいるか
	if (IsFly) {
		Fly(delta_time);
	}
	else {
		//エネルギーチャージ
		if (playerstate_->enargy() < 100) {
			playerstate_->addEnargy(delta_time * 0.5f);
		}
		//バーニアエフェクトの停止
		gsStopEffect(effectVernierSS1);
		gsStopEffect(effectVernierSS2);
		gsStopEffect(effectVernierS1);
		gsStopEffect(effectVernierS2);
		gsStopEffect(effectVernierL1);
		gsStopEffect(effectVernierL2);
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

	IsJumpTime -= delta_time;
	if (IsJumpTime < 0.0f) {
		IsJump = true;
	}

	//装備している武器に応じてUIのサイズを変える
	weaponSilhouetteSize();

	//エフェクトの位置などの更新
	effectUpdate(delta_time);
}

void Player::effectUpdate(float delta_time) {

	GSmatrix4 world;
	GSmatrix4 local_matrix;

	switch (vernierstate_)
	{
	case Player::VernierState::up:

		local_matrix = GSmatrix4::TRS(GSvector3{ -0.1,-0.04,-0.2 }, GSquaternion::euler(GSvector3{ 110.0f,30.0f,0.0f }), GSvector3{ 0.5f,0.5f,0.5f });
		world = local_matrix * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(effectVernierL1, &world);

		local_matrix = GSmatrix4::TRS(GSvector3{ 0.1,-0.04,-0.2 }, GSquaternion::euler(GSvector3{ 110.0f,-30.0f,0.0f }), GSvector3{ 0.5f,0.5f,0.5f });
		world = local_matrix * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(effectVernierL2, &world);
		break;
	case Player::VernierState::hover:
		local_matrix = GSmatrix4::TRS(GSvector3{ -0.1,-0.04,-0.2 }, GSquaternion::euler(GSvector3{ 110.0f,30.0f,0.0f }), GSvector3{ 1.0f,1.0f,1.0f });
		world = local_matrix * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(effectVernierS1, &world);

		local_matrix = GSmatrix4::TRS(GSvector3{ 0.1,-0.04,-0.2 }, GSquaternion::euler(GSvector3{ 110.0f,-30.0f,0.0f }), GSvector3{ 1.0f,1.0f,1.0f });
		world = local_matrix * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(effectVernierS2, &world);
		break;
	case Player::VernierState::down:
		local_matrix = GSmatrix4::TRS(GSvector3{ -0.1,-0.06,-0.2 }, GSquaternion::euler(GSvector3{ 110.0f,30.0f,0.0f }), GSvector3{ 1.0f,1.0f,1.0f });
		world = local_matrix * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(effectVernierSS1, &world);

		local_matrix = GSmatrix4::TRS(GSvector3{ 0.1,-0.06,-0.2 }, GSquaternion::euler(GSvector3{ 110.0f,-30.0f,0.0f }), GSvector3{ 1.0f,1.0f,1.0f });
		world = local_matrix * mesh_.BoneMatrices(4);
		gsSetEffectMatrix(effectVernierSS2, &world);
		break;
	}

	//一定値以下かつ死亡処理に入っていない場合
	if (HPReductionFrag && state_ != Player::State::Die) {

		DastMakeTimer -= delta_time;

		if (DastMakeTimer <= 0) {

			gsStopEffect(effectDast);

			Dastmakepos = GSvector3{ (float)gsRandf(-0.5,0.5),(float)gsRandf(-1,1) ,(float)gsRandf(-0.5,0.5) } + pos;

			Dastmakepos.y += PlayerHeight;

			effectDast = gsPlayEffect(Effect_FootDust, &Dastmakepos);
			GScolor4 DasteffectColor = GScolor4(0, 0, 0, 1);
			gsSetEffectColor(effectDast, &DasteffectColor);
			DastMakeTimer = 30.0f;
		}
	}

	if (EXSkill_) {
		//EXバフのエフェクト
		GSmatrix4 local_matrix = GSmatrix4::TRS(GSvector3{ 0,1,0 }, GSquaternion::euler(GSvector3{ 0.0f,0.0f,0.0f }), GSvector3{ 1.0f,1.0f,1.0f });;
		GSmatrix4 EXbuffworld = local_matrix * transform_.localToWorldMatrix();
		gsSetEffectMatrix(effectExbuff, &EXbuffworld);
		gsSetEffectMatrix(effectaura, &EXbuffworld);
	}

	//飛んでいないかつ移動状態にあるとき砂埃を発生させる
	if (!IsFly && (velocity_.x != 0.0f || velocity_.z != 0.0f)) {

		FootDastMakeTimer -= delta_time;

		if (FootDastMakeTimer <= 0) {
			//足元に砂埃エフェクト生成
			effectFootDast = gsPlayEffect(Effect_FootDust, &pos);
			GScolor4 FootFasteffectColor = GScolor4(0.6, 0.6, 0.6, 1);
			gsSetEffectColor(effectFootDast, &FootFasteffectColor);
			//生成クールタイム
			FootDastMakeTimer = 30.0f;
		}
	}
}

//描画
void Player::draw()const {

	if (!NotDrawMesh) {

		if (EXSkill_) {
			// ディザ半透明の設定を取得（退避しておく）
			float transparency = gsGetDitheredTransparency();

			// 現在の乗算カラーを取得（退避しておく）
			GScolor current_color;
			glGetFloatv(GL_CURRENT_COLOR, current_color);

			// ディザ半透明の設定　0.0f（透明）～1.0f（不透明）
			gsSetDitheredTransparency(meshAlpha);
			glSecondaryColor3fv(GScolor{ 0.8f,0.1f,0.1f,1.0f });

			//メッシュの描画
			mesh_.Draw();
			//武器を描画
			draw_weapon();

			// ディザ半透明をを復帰する
			gsSetDitheredTransparency(transparency);
			// 乗算カラーを復帰する
			glColor4fv(current_color);
			// 加算カラーを復帰する
			glSecondaryColor3fv(GScolor{ 0.0f,0.0f,0.0f,1.0f });
		}
		else {

			// ディザ半透明の設定を取得（退避しておく）
			float transparency = gsGetDitheredTransparency();

			// 現在の乗算カラーを取得（退避しておく）
			GScolor current_color;
			glGetFloatv(GL_CURRENT_COLOR, current_color);
			// 現在の加算カラーの取得（退避しておく）
			GScolor current_secondary_color;
			glGetFloatv(GL_CURRENT_SECONDARY_COLOR, current_secondary_color);

			// ディザ半透明の設定　0.0f（透明）～1.0f（不透明）
			gsSetDitheredTransparency(meshAlpha);

			//メッシュの描画
			mesh_.Draw();
			//武器を描画
			draw_weapon();

			// ディザ半透明をを復帰する
			gsSetDitheredTransparency(transparency);
			// 乗算カラーを復帰する
			glColor4fv(current_color);
			// 加算カラーを復帰する
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

	//レティクルの描画
	static const GSrect    reticle_rect{ 0, 0, 32, 32 };
	static const GSvector2 reticle_position{ screenwidtht / 2, screenheight / 2 };
	static const GSvector2 reticle_center{ 16,16 };
	gsDrawSprite2D(Texture_Reticle, &reticle_position, &reticle_rect, &reticle_center, NULL, NULL, 0.0f);
}

//HPバーの描画
void Player::drawHPBer()const {

	//HPバーのサイズ
	float maxhp = playerstate_->maxHP();
	float hp = playerstate_->hp();
	HPBarScale = (maxhp - hp) / maxhp;
	HPBarScale = CLAMP(HPBarScale, 0, 1);

	//プレイヤーのHP
	gsTextPos(150, 890);
	gsDrawText("HP:");

	//HPバー(青)
	gsDrawSprite2D(Texture_HP, &HPposition, &HPRect,
		NULL, &HPColor, &HPScale, 0.0f);

	//HPバー(灰)
	GSvector2 HPBackScale{ HPBarScale,1 };
	gsDrawSprite2D(Texture_HPBack, &HPBackposition, &HPBackRect,
		NULL, &HPBackColor, &HPBackScale, 180.0f);
}

//EXスキルバーの描画
void Player::drawEXBer()const {
	//必殺技のゲージ
	gsTextPos(180, 920);
	gsDrawText("必殺ゲージ:");
	int EXenargy = playerstate_->exSkillPoint();

	if (EXenargy < 100) {
		//下地
		gsDrawSprite2D(Texture_EX1, &EXposition, &EXRect, NULL, &EXColor, &EXScale, 0.0f);

		enargyBarScale = { (float)EXenargy / 100, 1.0 };
		//可動
		gsDrawSprite2D(Texture_EX2, &EXposition, &EXRect, NULL, &EXColor, &enargyBarScale, 0.0f);
	}
	else if (EXenargy >= 100 && EXenargy < 200) {
		//下地
		gsDrawSprite2D(Texture_EX2, &EXposition, &EXRect, NULL, &EXColor, &EXScale, 0.0f);

		enargyBarScale = { ((float)EXenargy - 100) / 100, 1.0 };
		//可動
		gsDrawSprite2D(Texture_EX3, &EXposition, &EXRect, NULL, &EXColor, &enargyBarScale, 0.0f);

		gsDrawSprite2D(Texture_EX2Ball, &EXBallposition1, &EXBallRect, NULL,
			&EXBallColor, &EXBallScale, 0.0f);
	}
	else if (EXenargy >= 200 && EXenargy < 300) {
		//下地
		gsDrawSprite2D(Texture_EX3, &EXposition, &EXRect, NULL, &EXColor, &EXScale, 0.0f);
		enargyBarScale = { ((float)EXenargy - 200) / 100, 1.0 };
		//可動
		gsDrawSprite2D(Texture_EX4, &EXposition, &EXRect, NULL, &EXColor, &enargyBarScale, 0.0f);

		gsDrawSprite2D(Texture_EX2Ball, &EXBallposition1, &EXBallRect, NULL,
			&EXBallColor, &EXBallScale, 0.0f);

		gsDrawSprite2D(Texture_EX3Ball, &EXBallposition2, &EXBallRect, NULL,
			&EXBallColor, &EXBallScale, 0.0f);
	}
	else {
		//下地
		gsDrawSprite2D(Texture_EX4, &EXposition, &EXRect, NULL, &EXColor, &EXScale, 0.0f);

		gsDrawSprite2D(Texture_EX2Ball, &EXBallposition1, &EXBallRect, NULL,
			&EXBallColor, &EXBallScale, 0.0f);

		gsDrawSprite2D(Texture_EX3Ball, &EXBallposition2, &EXBallRect, NULL,
			&EXBallColor, &EXBallScale, 0.0f);

		gsDrawSprite2D(Texture_EX4Ball, &EXBallposition3, &EXBallRect, NULL,
			&EXBallColor, &EXBallScale, 0.0f);
	}
}

//武器のシルエットの描画
void Player::drawWeaponSilhouette()const {
	gsDrawSprite2D(Texture_BeamLifle, &BeamLiflePosition, &BeamLifleRect, NULL,
		&BeamLifleColor, &BeamLifleScale, 0.0f);

	gsDrawSprite2D(Texture_BeamMagnum, &BeamMagnumPosition, &BeamMagnumRect, NULL,
		&BeamMagnumColor, &BeamMagnumScale, 0.0f);

	gsDrawSprite2D(Texture_Bazooka, &BazookaPosition, &BazookaRect, NULL,
		&BazookaColor, &BazookaScale, 0.0f);

	//弾
	gsDrawSprite2D(Texture_Bullet, &BulletPosition, &BulletRect, NULL,
		&BulletColor, &BulletScale, 0.0f);

	//マガジン
	gsDrawSprite2D(Texture_Magajin, &MagajinPosition, &MagajinRect, NULL,
		&MagajinColor, &MagajinScale, 0.0f);

	//マガジン数や弾数表示
	switch (playerstate_->gunstate_())
	{
	case PlayerState::GunState::Beamlifl:
		gsTextPos(1650, BeamLiflePosition.y);
		gsDrawText("*∞");
		gsTextPos(1750, BeamLiflePosition.y);
		gsDrawText("*∞");
		break;
	case PlayerState::GunState::BeamMagnumBullet:
		gsTextPos(1650, BeamMagnumPosition.y);
		gsDrawText("*%d", playerstate_->beamMagnumBullet());
		gsTextPos(1750, BeamMagnumPosition.y);
		gsDrawText("*%d", playerstate_->beamMagnamMagazin());
		break;
	case PlayerState::GunState::BazookaBullet:
		gsTextPos(1650, BazookaPosition.y);
		gsDrawText("*%d", playerstate_->bazookaBullet());
		gsTextPos(1750, BazookaPosition.y);
		gsDrawText("*%d", playerstate_->bazookaMagazin());
		break;
	}
}

//装備している向きに応じてUIのサイズとα値を変える
void Player::weaponSilhouetteSize() {
	//装備している銃に応じてUIのα値を変える
	switch (playerstate_->gunstate_())
	{
	case PlayerState::GunState::Beamlifl:
		BeamLifleColor.a = 1.0f;
		BeamMagnumColor.a = 0.5f;
		BazookaColor.a = 0.5f;
		BulletPosition.y = MagajinPosition.y = BeamLiflePosition.y;

		BeamLifleScale = AssignmentBeamLifleScale * magnification;
		BeamMagnumScale = AssignmentBeamMagnumScale;
		BazookaScale = AssignmentBazookaScale;

		break;
	case PlayerState::GunState::BeamMagnumBullet:
		BeamLifleColor.a = 0.5f;
		BeamMagnumColor.a = 1.0f;
		BazookaColor.a = 0.5f;
		BulletPosition.y = MagajinPosition.y = BeamMagnumPosition.y;

		BeamLifleScale = AssignmentBeamLifleScale;
		BeamMagnumScale = AssignmentBeamMagnumScale * magnification;
		BazookaScale = AssignmentBazookaScale;

		break;
	case PlayerState::GunState::BazookaBullet:
		BeamLifleColor.a = 0.5f;
		BeamMagnumColor.a = 0.5f;
		BazookaColor.a = 1.0f;
		BulletPosition.y = MagajinPosition.y = BazookaPosition.y;

		BeamLifleScale = AssignmentBeamLifleScale;
		BeamMagnumScale = AssignmentBeamMagnumScale;
		BazookaScale = AssignmentBazookaScale * magnification;

		break;
	}
}

//スラスター残量バーの描画
void Player::drawThrusterBer() const {
	//スラスター残量
	if (playerstate_->enargy() < 100) {

		gsDrawSprite2D(Texture_Buster2, &Thrusterposition, &ThrusterRect, NULL,
			&ThrusterColor, &ThrusterScale, 0.0f);

		ThrusterBackScale.x = (playerstate_->MaxEnargy() - playerstate_->enargy()) / playerstate_->MaxEnargy();

		gsDrawSprite2D(Texture_Buster1, &ThrusterBackposition, &ThrusterBackRect, NULL,
			&ThrusterBackColor, &ThrusterBackScale, 180.0f);
	}
}

//武器の描画
void Player::draw_weapon()const {

	//飛んでいるか
	if (IsFly) {
		//手の位置に銃を描画
		glPushMatrix();
		glMultMatrixf(mesh_.BoneMatrices(37));
		gsDrawMesh(Mesh_Weapon);
		glPopMatrix();
	}
	else if (!IsFly) {
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

	if (other.tag() == "EnemyBulletTag" && !collisionInvalid && !damageFrag_) {
		int damage = static_cast<BasicAttackCollider*>(&other)->GetAttackValue();

		//ダメージ処理
		playerstate_->AddHP(-damage);

		if (playerstate_->hp() <= 0) {

			//バーニア停止
			gsStopEffect(effectVernierL1);
			gsStopEffect(effectVernierL2);
			gsStopEffect(effectVernierS1);
			gsStopEffect(effectVernierS2);
			gsStopEffect(effectVernierSS1);
			gsStopEffect(effectVernierSS2);

			//必殺時に出るエフェクト
			gsStopEffect(effectExbuff);
			gsStopEffect(effectaura);

			effectExplosion = gsPlayEffect(Effect_ExplosionL, &pos);
			NotDrawMesh = true;
			state_ = Player::State::Die;
			return;
		}
		else {

			//ターゲット方向のベクトルを求める
			GSvector3 to_target = other.transform().position() - pos;
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
			effectHit = gsPlayEffect(Effect_Hit, &pos);
			damageFrag_ = true;
			meshAlpha = 0.5f;
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
	velocity = velocity.normalized() * walkSpeed * delta_time;

	GSint motion;

	if (IsFly) 	motion = Motion_Idle_GunAir;
	else motion = Motion_Idle_GunEarth;

	//移動しているか？
	if (velocity.length() != 0.f) {
		//向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity), 12.0f * delta_time);
		transform_.rotation(rotation);

		//移動中のモーションにする
		if (IsFly) motion = Motion_WarkF_GunAir;
		else motion = Motion_WarkF_GunEarth;
	}
	//モーションの変更
	change_state(State::Move, motion);

	if (gsGetKeyState(GKEY_W)) {
		if (gsGetKeyState(GKEY_LSHIFT)) {

			if (IsFly) {
				motion = Motion_RunF_GunAir;
				forward_speed = walkSpeed * skyRunSpeed_;
			}
			else {
				motion = Motion_RunF_GunEarth;
				forward_speed = walkSpeed * groundRunSpeed_;
			}
		}
		else {
			if (IsFly) {
				motion = Motion_WarkF_GunAir;
				forward_speed = walkSpeed * skyMoveSpeed_;
			}
			else {
				motion = Motion_WarkF_GunEarth;
				forward_speed = walkSpeed;
			}
		}
	}
	if (gsGetKeyState(GKEY_S)) {
		if (gsGetKeyState(GKEY_LSHIFT) && !IsFly) {

			forward_speed = -walkSpeed * groundRunSpeed_;
			if (!IsFly) motion = Motion_RunB_GunEarth;

		}
		else {
			if (IsFly) {
				motion = Motion_WarkB_GunAir;
				forward_speed = -walkSpeed * skyMoveSpeed_;
			}
			else {
				motion = Motion_WarkB_GunEarth;
				forward_speed = -walkSpeed;
			}
		}
	}
	if (gsGetKeyState(GKEY_A)) {
		if (gsGetKeyState(GKEY_LSHIFT)) {

			if (IsFly) {
				motion = Motion_RunL_GunAir;
				side_speed = walkSpeed * skyRunSpeed_;
			}
			else {
				motion = Motion_RunL_GunEarth;
				side_speed = walkSpeed * groundRunSpeed_;
			}
		}
		else {
			if (IsFly) {
				motion = Motion_WarkL_GunAir;
				side_speed = walkSpeed * skyMoveSpeed_;
			}
			else {
				motion = Motion_WarkL_GunEarth;
				side_speed = walkSpeed;
			}
		}
	}
	if (gsGetKeyState(GKEY_D)) {
		if (gsGetKeyState(GKEY_LSHIFT)) {
			if (IsFly) {
				motion = Motion_RunR_GunAir;
				side_speed = -walkSpeed * skyRunSpeed_;
			}
			else {
				motion = Motion_RunR_GunEarth;
				side_speed = -walkSpeed * groundRunSpeed_;
			}
		}
		else {
			if (IsFly) {
				motion = Motion_WarkR_GunAir;
				side_speed = -walkSpeed * skyMoveSpeed_;
			}
			else {
				motion = Motion_WarkR_GunEarth;
				side_speed = -walkSpeed;
			}
		}
	}
	//移動状態にする
	change_state(State::Move, motion);

	velocity_.x = side_speed;
	velocity_.z = forward_speed;


	//平行移動する
	transform_.translate(side_speed * delta_time, 0.f, forward_speed * delta_time);

	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && IsJump && !IsFly) {
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

	if (IsFly) {
		//射撃ステータスに移行
		change_state(State::ShootAttack, Motion_Attack2_GunAir);
		//攻撃可能フラグをオン
		IsAttack = true;
		//移動ボタンが押されたら移動中の攻撃にステータスを変える
		if (gsGetKeyState(GKEY_W)) change_state(State::MoveShootAttack, Motion_Attack1_GunAir);
		if (gsGetKeyState(GKEY_S)) change_state(State::MoveShootAttack, Motion_Attack1_GunAir);
		if (gsGetKeyState(GKEY_A)) change_state(State::MoveShootAttack, Motion_Attack1_GunAir);
		if (gsGetKeyState(GKEY_D)) change_state(State::MoveShootAttack, Motion_Attack1_GunAir);
	}
	else if (!IsFly) {

		//射撃ステータスに移行
		change_state(State::ShootAttack, Motion_Attack_GunEarth);
		//攻撃可能フラグをオン
		IsAttack = true;
		//移動ボタンが押されたら移動中の攻撃にステータスを変える
		if (gsGetKeyState(GKEY_W)) change_state(State::MoveShootAttack, Motion_MAttackF_GunEarth);
		if (gsGetKeyState(GKEY_S)) change_state(State::MoveShootAttack, Motion_MAttackB_GunEarth);
		if (gsGetKeyState(GKEY_A)) change_state(State::MoveShootAttack, Motion_MAttackL_GunEarth);
		if (gsGetKeyState(GKEY_D)) change_state(State::MoveShootAttack, Motion_MAttackR_GunEarth);
	}
}

//射撃
void Player::shoot(float delta_time) {

	//撃っている途中で０になったらステータス移行
	JudgementBullet();

	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && IsJump && !IsFly) {
		change_state(State::JumpStart, Motion_JumpStart_GunEarth);
		velocity_.y = JumpHight;
	}

	//弾生成
	if (IsAttack)generate_bullet();

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

		if (IsFly)change_state(State::Move, Motion_Idle_GunAir);
		else if (!IsFly)change_state(State::Move, Motion_Idle_GunEarth);
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
	explosionTimer -= delta_time;

	//エフェクト再生終了したら
	if (explosionTimer < 0 && !DieFrag) {

		gsStopEffect(effectExplosion);

		//死亡フラグを上げる
		DieFrag = true;

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
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = walkSpeed * groundRunSpeed_;
		else forward_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_S)) {
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = -walkSpeed * groundRunSpeed_;
		else forward_speed = -walkSpeed;
	}
	if (gsGetKeyState(GKEY_A)) {
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = walkSpeed * groundRunSpeed_;
		else side_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_D)) {
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = -walkSpeed * groundRunSpeed_;
		else side_speed = -walkSpeed;
	}

	velocity_.z = forward_speed;
	velocity_.x = side_speed;
	transform_.translate(side_speed * delta_time, 0.f, forward_speed * delta_time);

	if (gsGetKeyTrigger(GKEY_SPACE)) {

		velocity_.y = 0.0f;
		IsFly = true;
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
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = walkSpeed * groundRunSpeed_;
		else forward_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_S)) {
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = -walkSpeed * groundRunSpeed_;
		else forward_speed = -walkSpeed;
	}
	if (gsGetKeyState(GKEY_A)) {
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = walkSpeed * groundRunSpeed_;
		else side_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_D)) {
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = -walkSpeed * groundRunSpeed_;
		else side_speed = -walkSpeed;
	}

	velocity_.z = forward_speed;
	velocity_.x = side_speed;
	transform_.translate(side_speed * delta_time, 0.f, forward_speed * delta_time);

	if (gsGetKeyTrigger(GKEY_SPACE)) {
		velocity_.y = 0.0f;
		IsFly = true;

		change_state(State::JumpEnd, Motion_JumpEnd_GunEarth);
	}
	ClampPos();
}

//ジャンプ終了
void Player::jump_end(float delta_time) {
	if (state_timer_ >= 3) {

		change_state(State::Move, Motion_Idle_GunEarth);

		IsJump = false;
		IsJumpTime = 15.0f;
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
		forward_speed = walkSpeed;

		if (IsFly)motion_ = Motion_Attack1_GunAir;
		else if (!IsFly)motion_ = Motion_MAttackF_GunEarth;
	}
	if (gsGetKeyState(GKEY_S)) {
		forward_speed = -walkSpeed;

		if (IsFly)motion_ = Motion_Attack1_GunAir;
		else if (!IsFly)motion_ = Motion_MAttackB_GunEarth;
	}
	if (gsGetKeyState(GKEY_A)) {
		side_speed = walkSpeed;

		if (IsFly)motion_ = Motion_Attack1_GunAir;
		else if (!IsFly)motion_ = Motion_MAttackL_GunEarth;
	}
	if (gsGetKeyState(GKEY_D)) {
		side_speed = -walkSpeed;

		if (IsFly)motion_ = Motion_Attack1_GunAir;
		else if (!IsFly)motion_ = Motion_MAttackR_GunEarth;
	}

	velocity_.x = side_speed;
	velocity_.z = forward_speed;

	transform_.translate(side_speed * delta_time, 0.f, forward_speed * delta_time);

	//立ち止まったら攻撃開始状態へ
	if (forward_speed == 0.0f && side_speed == 0.0f) {
		if (IsFly)change_state(State::ShootAttack, Motion_Attack2_GunAir);
		else if (!IsFly)change_state(State::ShootAttack, Motion_Attack_GunEarth);
	}

	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && IsJump && !IsFly) {
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

	//playerstate_->addEnargy(-delta_time * 0.1f);

	float UpSpeed{ 0.0f };

	if (gsGetKeyState(GKEY_SPACE) && transform_.position().y < 51) {
		UpSpeed += walkSpeed;
		vernierstate_ = Player::VernierState::up;
	}
	else if (gsGetKeyState(GKEY_LCONTROL)) {
		UpSpeed -= walkSpeed;
		vernierstate_ = Player::VernierState::down;
	}
	else {
		vernierstate_ = Player::VernierState::hover;
	}

	//エフェクト再生
	if (ComparisonVernierstate_ != vernierstate_) {
		switch (vernierstate_)
		{
		case Player::VernierState::up:
			gsStopEffect(effectVernierS1);
			gsStopEffect(effectVernierS2);
			gsStopEffect(effectVernierSS1);
			gsStopEffect(effectVernierSS2);
			effectVernierL1 = gsPlayEffect(Effect_VernierBL, &pos);
			effectVernierL2 = gsPlayEffect(Effect_VernierBL, &pos);
			break;
		case Player::VernierState::hover:
			gsStopEffect(effectVernierL1);
			gsStopEffect(effectVernierL2);
			gsStopEffect(effectVernierSS1);
			gsStopEffect(effectVernierSS2);
			effectVernierS1 = gsPlayEffect(Effect_VernierBS, &pos);
			effectVernierS2 = gsPlayEffect(Effect_VernierBS, &pos);
			break;
		case Player::VernierState::down:
			gsStopEffect(effectVernierL1);
			gsStopEffect(effectVernierL2);
			gsStopEffect(effectVernierS1);
			gsStopEffect(effectVernierS2);
			effectVernierSS1 = gsPlayEffect(Effect_VernierBSS, &pos);
			effectVernierSS2 = gsPlayEffect(Effect_VernierBSS, &pos);
			break;
		}
		ComparisonVernierstate_ = vernierstate_;
	}

	transform_.translate(0, UpSpeed * delta_time, 0);

	if (playerstate_->enargy() <= 0.0f)IsFly = false;
}

void Player::exSkill(float delta_time) {

	if (ExSkillRrocess) {

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
			collisionInvalid = true;
			playerState_()->setExSkillPoint(-300);
		}
		ExSkillRrocess = false;
	}

	//継続時間
	EXskillTimer_ -= delta_time;

	if (EXskillTimer_ <= 0) {

		gsStopEffect(effectExbuff);
		gsStopEffect(effectaura);

		//プレイヤーのステータスを発動前に戻す
		playerState_()->resetEXSkill();

		//ファンネル撤退
		if (units_ != NULL)units_->changeFrag(true);
		//無敵解除
		collisionInvalid = false;

		//初期化
		EXSkill_ = EXskillfinish_ = false;
		EXskillTimer_ = assignmentExSkillTimer_;
	}
}

void Player::MakeUnit() {
	GSvector3 makepos = pos;
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
		if (IsFly)IsFly = false;
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
	GC = static_cast<GunControl*>(world_->find_actor("GunControl"));

	GC->Fire();
	IsAttack = false;
}


//モーション中に弾を生成する
void Player::can_bullet() {

	//マウスクリックで射撃
	if (gsGetMouseButtonState(GMOUSE_BUTTON_1))	generate_bullet();
}

//アニメーションイベントの設定
void Player::SetAnimationEvent() {

	mesh_.AddEvent(Motion_MAttackF_GunEarth, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 8, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 8, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 3, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 4, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 5, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 6, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 7, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 8, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 8, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, 0, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 8, [this] {can_bullet(); });
}

void Player::ClampPos() {
	GSvector3 position = transform_.position();
	position.x = CLAMP(position.x, -88.0f, 210.0f);
	position.z = CLAMP(position.z, -21.0f, 38.0f);
	transform_.position(position);
}

