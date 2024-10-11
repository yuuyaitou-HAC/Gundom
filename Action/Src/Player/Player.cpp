#include "Player/Player.h"
#include "PlayerState.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Camera/CameraTPS.h"
#include "Gun/GunControl.h"
#include "PlayerBullet/AttackRange.h"

//モーション番号
enum {

	//アイドルモーション
	Motion_Idle_GunEarth = 0,
	Motion_Idle_GunAir = 1,
	Motion_Idle_SaberEarth = 2,

	//銃装備時の移動
	Motion_WarkF_GunEarth = 3,
	Motion_WarkB_GunEarth = 4,
	Motion_WarkL_GunEarth = 5,
	Motion_WarkR_GunEarth = 6,

	//銃装備時の空中移動
	Motion_WarkF_GunAir = 7,
	Motion_WarkB_GunAir = 8,
	Motion_WarkL_GunAir = 9,
	Motion_WarkR_GunAir = 10,

	//剣装備時の移動
	Motion_WarkF_SaberEarth = 11,
	Motion_WarkB_SaberEarth = 12,
	Motion_WarkL_SaberEarth = 13,
	Motion_WarkR_SaberEarth = 14,

	//銃装備時の移動攻撃
	Motion_MAttackF_GunEarth = 15,
	Motion_MAttackB_GunEarth = 16,
	Motion_MAttackL_GunEarth = 17,
	Motion_MAttackR_GunEarth = 18,

	//銃装備時の走り
	Motion_RunF_GunEarth = 19,
	Motion_RunB_GunEarth = 20,
	Motion_RunL_GunEarth = 21,
	Motion_RunR_GunEarth = 22,

	//銃装備時の空中高速移動
	Motion_RunF_GunAir = 23,
	Motion_RunL_GunAir = 24,
	Motion_RunR_GunAir = 25,

	//剣装備時の走り
	Motion_RunF_SaberEarth = 26,
	Motion_RunB_SaberEarth = 27,
	Motion_RunL_SaberEarth = 28,
	Motion_RunR_SaberEarth = 29,

	//銃装備時のその場での攻撃
	Motion_Attack_GunEarth = 30,

	//銃装備時のその場での攻撃(空中)
	Motion_Attack1_GunAir = 31,
	Motion_Attack2_GunAir = 32,

	//剣装備時の攻撃(コンボ含む)
	Motion_Attack1_SubarEath = 33,
	Motion_Attack2_SubarEath = 34,
	Motion_Attack3_SubarEath = 35,

	//銃装備時のジャンプ
	Motion_JumpStart_GunEarth = 36,
	Motion_Jump_GunEarth = 37,
	Motion_JumpEnd_GunEarth = 38,

	//剣装備時のジャンプ
	Motion_Jump_SaberEarth = 39,

	//銃装備時の着地
	Motion_Landing_GunEarth = 40,

	//地上にいるときの武器の切り替え(銃)
	Motion_ChangeWepon1_GunEarth = 41,
	Motion_ChangeWepon2_GunEarth = 42,

	//地上にいるときの武器の切り替え(剣)
	Motion_ChangeWepon1_SaberEarth = 43,
	Motion_ChangeWepon2_SaberEarth = 44,

	//銃装備時の地上でダメージを受けたとき
	Motion_Damage_GunEarth = 45,

	//銃装備時の空中でダメージを受けたとき
	Motion_Damage_GunAir = 46,

	//剣装備時の地上でダメージを受けたとき
	Motion_Damage1_SaberEarth = 47,
	Motion_Damage2_SaberEarth = 48,

	//銃装備時に死んだ
	Motion_Die_GunEarth = 49,

	//銃装備時に空中で死んだ
	Motion_Die_GunAir = 50,

	//剣装備時に死んだ
	Motion_Die_SaberEarth = 51,
};

//自分の高さ
const float PlayerHeight{ 1.f };
//衝突判定用の半径
const float PlayerRadius{ 0.5f };
//足元のオフセット
const float FootOffset{ 0.1f };
//重力値
const float Gravity{ -0.016f };

//走るときの速さ(倍率)
const float runSpeed{ 2.0f };

//ジャンプ時の高さ
const float JumpHight{ 0.3f };

//コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player,Mesh_Player,Mesh_Player,Motion_Idle_GunEarth,true },
	motion_{ Motion_Idle_GunEarth },
	motion_loop_{ true },
	state_{ State::Move },
	state_timer_{ 0.f },
	AttackChange{ false },//false:射撃 true:斬撃 
	CanBullet{ 10 },
	CameraSensitivity{ 2.0f }
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
	//メッシュの変換行列を初期化
	mesh_.Transform(transform_.localToWorldMatrix());

	//プレイヤーのステータス生成
	playerstate_ = new PlayerState();

	//飛べるかどうか
	IsFly = false;

	//パワーを代入
	FlyPower = playerState_()->Enargy();

	motion_ = 0;

	//アニメーション中のイベント設定
	SetAnimationEvent();

}

//デストラクタ
Player::~Player() {

	//プレイヤーステータス削除
	delete playerstate_;
}

//更新
void Player::update(float delta_time) {

	walkSpeed = playerstate_->MoveSpeed();

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
		//現在のパワーを代入

		FlyPower = CLAMP(FlyPower, 0.0f, 100.0f);

		//時間をかけて回復
		FlyPower += delta_time * 0.5f;

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
	if (IsJumpTime < 0.0f)
	{
		IsJump = true;
	}

	SQRT();

}

//描画
void Player::draw()const {
	//メッシュの描画
	mesh_.Draw();
	//武器を描画
	draw_weapon();



}

void Player::draw_gui() const {

	//プレイヤーの座標表示
	gsTextPos(300, 450);
	//gsDrawText("PlayerPos= %f %f %f", transform_.position().x, transform_.position().y, transform_.position().z);
	gsDrawText("WalkSpeed = %f", walkSpeed);
}



//武器の描画
void Player::draw_weapon()const {

	//飛んでいるか
	if (IsFly) {

		//空中剣
		if (AttackChange) {

			//腰位置に銃を描画
			glPushMatrix();
			glMultMatrixf(mesh_.BoneMatrices(52));
			glRotatef(-90, 1, 0, 0);
			gsDrawMesh(Mesh_Weapon);
			glPopMatrix();


			//手の位置にビームサーベルを描画
			glPushMatrix();
			glMultMatrixf(mesh_.BoneMatrices(40));//32
			gsDrawMesh(Mesh_BeamSbred);
			glPopMatrix();

		}
		//空中銃
		else if (!AttackChange) {

			//手の位置に銃を描画
			glPushMatrix();
			glMultMatrixf(mesh_.BoneMatrices(37));
			gsDrawMesh(Mesh_Weapon);
			glPopMatrix();


			//バックパックにビームサーベルを描画
			glPushMatrix();
			glMultMatrixf(mesh_.BoneMatrices(51));
			glRotatef(-200, 0, 0, 1);
			gsDrawMesh(Mesh_BeamSbred);
			glPopMatrix();

		}


	}
	else if (!IsFly) {

		//地上剣
		if (AttackChange) {

			//腰位置に銃を描画
			glPushMatrix();
			glMultMatrixf(mesh_.BoneMatrices(52));
			glRotatef(-90, 1, 0, 0);
			gsDrawMesh(Mesh_Weapon);
			glPopMatrix();


			//手の位置にビームサーベルを描画
			glPushMatrix();
			glMultMatrixf(mesh_.BoneMatrices(40));//32
			gsDrawMesh(Mesh_BeamSbred);
			glPopMatrix();

		}
		//地上銃
		else if (!AttackChange) {

			//手の位置に銃を描画
			glPushMatrix();
			glMultMatrixf(mesh_.BoneMatrices(36));
			gsDrawMesh(Mesh_Weapon);
			glPopMatrix();


			//バックパックにビームサーベルを描画
			glPushMatrix();
			glMultMatrixf(mesh_.BoneMatrices(51));
			glRotatef(-200, 0, 0, 1);
			gsDrawMesh(Mesh_BeamSbred);
			glPopMatrix();

		}


	}

}

//衝突リアクション
void Player::react(Actor& other) {
	//ここに衝突判定の処理があるとする
	if (state_ == State::Damage)return;
	//敵の攻撃判定と衝突したか？
	if (other.tag() == "EnemyAttackTag") {
		//ターゲット方向のベクトルを求める
		GSvector3 to_target = other.transform().position() - transform().position();
		//ｙ成分は無効にする
		to_target.y = 0.f;
		//ターゲット方向と逆方向にノックバックする移動量を求める
		velocity_ = -to_target.getNormalized() * 0.4f;
		//ダメージ状態に遷移する
		if (IsFly) {
			change_state(State::Damage, Motion_Damage_GunAir, false);
			return;
		}if (AttackChange) {
			change_state(State::Damage, Motion_Damage2_SaberEarth, false);
			return;
		}
		else if (!AttackChange) {
			change_state(State::Damage, Motion_Damage_GunEarth, false);
			return;
		}
	}
	//敵と衝突したか？
	if (other.tag() == "EnemyTag") {
		//アクター同士が重ならないように補正する
		collide_actor(other);
	}
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
		attack(delta_time);
		break;
	case Player::State::SlashAttack:
		slash(delta_time);
		break;
	case Player::State::SecondSlash:
		Secondslash(delta_time);
		break;
	case Player::State::ThirdSlash:
		Thirdslash(delta_time);
		break;
	case Player::State::Damage:
		damage(delta_time);
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
	case Player::State::MoveSlashAttack:
		move_slash(delta_time);
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

	if (gsGetKeyTrigger(GKEY_Q)) {

		if (AttackChange) {
			AttackChange = false;
		}
		else {
			AttackChange = true;
		}
	}

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };

	//銃撃
	if (gsGetMouseButtonState(GMOUSE_BUTTON_1) && !AttackChange) {
		AttackJudgment();
		return;
	}

	//斬撃
	if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1) && AttackChange) {
		SlashProcessing();
		return;
	}

	//PADの攻撃方法


	GSvector3 velocity{ 0.f,0.f,0.f };
	velocity = velocity.normalized() * walkSpeed * delta_time;

	GSint motion;

	if (IsFly) {
		motion = Motion_Idle_GunAir;

	}
	else {
		if (!AttackChange) {
			motion = Motion_Idle_GunEarth;
		}
		else if (AttackChange) {
			motion = Motion_Idle_SaberEarth;
		}
	}

	//移動しているか？
	if (velocity.length() != 0.f) {
		//向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity), 12.0f * delta_time);
		transform_.rotation(rotation);
		//移動中のモーションにする
		if (IsFly) {
			motion = Motion_WarkF_GunAir;
		}
		else {
			if (!AttackChange) {
				motion = Motion_WarkF_GunEarth;
			}
			else if (AttackChange) {
				motion = Motion_WarkF_SaberEarth;
			}
		}

	}
	//モーションの変更
	change_state(State::Move, motion);

	if (gsGetKeyState(GKEY_W)) {
		if (gsGetKeyState(GKEY_LSHIFT)) {
			forward_speed = walkSpeed * runSpeed;

			if (IsFly) {
				motion = Motion_RunF_GunAir;
			}
			else {

				if (!AttackChange) {
					motion = Motion_RunF_GunEarth;
				}
				else if (AttackChange) {
					motion = Motion_RunF_SaberEarth;
				}
			}

		}
		else {
			forward_speed = walkSpeed;

			if (IsFly) {
				motion = Motion_WarkF_GunAir;
			}
			else {

				if (!AttackChange) {
					motion = Motion_WarkF_GunEarth;
				}
				else if (AttackChange) {
					motion = Motion_WarkF_SaberEarth;
				}
			}
		}
	}
	if (gsGetKeyState(GKEY_S)) {
		if (gsGetKeyState(GKEY_LSHIFT) && !IsFly) {
			forward_speed = -walkSpeed * runSpeed;

			if (!IsFly) {

				if (!AttackChange) {
					motion = Motion_RunB_GunEarth;
				}
				else if (AttackChange) {
					motion = Motion_RunB_SaberEarth;
				}
			}


		}
		else {
			forward_speed = -walkSpeed;
			if (IsFly) {
				motion = Motion_WarkB_GunAir;
			}
			else {

				if (!AttackChange) {
					motion = Motion_WarkB_GunEarth;
				}
				else if (AttackChange) {
					motion = Motion_WarkB_SaberEarth;
				}
			}
		}
	}
	if (gsGetKeyState(GKEY_A)) {
		if (gsGetKeyState(GKEY_LSHIFT)) {
			side_speed = walkSpeed * runSpeed;
			if (IsFly) {
				motion = Motion_RunL_GunAir;
			}
			else {
				if (!AttackChange) {
					motion = Motion_RunL_GunEarth;
				}
				else if (AttackChange) {
					motion = Motion_RunL_SaberEarth;
				}
			}
		}
		else {
			side_speed = walkSpeed;
			if (IsFly) {
				motion = Motion_WarkL_GunAir;
			}
			else {

				if (!AttackChange) {
					motion = Motion_WarkL_GunEarth;
				}
				else if (AttackChange) {
					motion = Motion_WarkL_SaberEarth;
				}
			}
		}
	}
	if (gsGetKeyState(GKEY_D)) {
		if (gsGetKeyState(GKEY_LSHIFT)) {
			side_speed = -walkSpeed * runSpeed;
			if (IsFly) {
				motion = Motion_RunR_GunAir;
			}
			else {

				if (!AttackChange) {
					motion = Motion_RunR_GunEarth;
				}
				else if (AttackChange) {
					motion = Motion_RunR_SaberEarth;
				}
			}
		}
		else {
			side_speed = -walkSpeed;
			if (IsFly) {
				motion = Motion_WarkR_GunAir;
			}
			else {

				if (!AttackChange) {
					motion = Motion_WarkR_GunEarth;
				}
				else if (AttackChange) {
					motion = Motion_WarkR_SaberEarth;
				}
			}
		}
	}
	//移動状態にする
	change_state(State::Move, motion);

	//マウスの左右方向で方向を変える
	int mx, my, mz;
	gsGetMouseVelocity(&mx, &my, &mz);
	float yaw = (float)-mx * 0.5f;
	transform_.rotate(0.f, yaw * delta_time, 0.f);

	//平行移動する
	transform_.translate(side_speed * delta_time, 0.f, forward_speed * delta_time);

	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && IsJump && !IsFly) {
		// ジャンプ開始状態へ
		if (AttackChange) {
			change_state(State::JumpStart, Motion_Jump_SaberEarth);
		}
		else if (!AttackChange) {
			change_state(State::JumpStart, Motion_JumpStart_GunEarth);
		}
		// ジャンプ
		velocity_.y = JumpHight;
		return;
	}

}

void Player::SQRT() {

	if (gsGetKeyState(GKEY_W) && gsGetKeyState(GKEY_D) ||
		gsGetKeyState(GKEY_W) && gsGetKeyState(GKEY_A) ||
		gsGetKeyState(GKEY_S) && gsGetKeyState(GKEY_D) ||
		gsGetKeyState(GKEY_S) && gsGetKeyState(GKEY_A)) {

	}

}

//弾が撃てるか
void Player::AttackJudgment() {

	//拡充のステータス時に各弾が０の時は何もしない
	//０でない時は撃つ
	if (playerState_()->gunstate_() == PlayerState::GunState::Beamlifl
		&& playerState_()->BeamBullet() > 0) {

		AttackProcessing();
	}
	if (playerState_()->gunstate_() == PlayerState::GunState::BeamMagnumBullet
		&& playerState_()->BeamMagnumBullet() > 0) {

		AttackProcessing();
	}
	if (playerState_()->gunstate_() == PlayerState::GunState::BazookaBullet
		&& playerState_()->BazookaBullet() > 0) {

		AttackProcessing();
	}
}

//攻撃処理の最中
void Player::AttackProcessing() {

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };

	if (forward_speed == 0.0f && side_speed == 0.0f) {

		if (IsFly) {
			change_state(State::ShootAttack, Motion_Attack1_GunAir);

			//移動ボタンが押されたら移動中の攻撃にステータスを変える
			if (gsGetKeyState(GKEY_W)) change_state(State::MoveShootAttack, Motion_Attack1_GunAir);
			if (gsGetKeyState(GKEY_S)) change_state(State::MoveShootAttack, Motion_Attack1_GunAir);
			if (gsGetKeyState(GKEY_A)) change_state(State::MoveShootAttack, Motion_Attack1_GunAir);
			if (gsGetKeyState(GKEY_D)) change_state(State::MoveShootAttack, Motion_Attack1_GunAir);

		}
		else if (!IsFly) {
			change_state(State::ShootAttack, Motion_Attack_GunEarth);

			//移動ボタンが押されたら移動中の攻撃にステータスを変える
			if (gsGetKeyState(GKEY_W)) change_state(State::MoveShootAttack, Motion_MAttackF_GunEarth);
			if (gsGetKeyState(GKEY_S)) change_state(State::MoveShootAttack, Motion_MAttackB_GunEarth);
			if (gsGetKeyState(GKEY_A)) change_state(State::MoveShootAttack, Motion_MAttackL_GunEarth);
			if (gsGetKeyState(GKEY_D)) change_state(State::MoveShootAttack, Motion_MAttackR_GunEarth);

		}
		IsAttack = true;
	}
}

//斬撃処理の最中
void Player::SlashProcessing() {

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };

	if (forward_speed == 0.0f && side_speed == 0.0f) {

		change_state(State::SlashAttack, Motion_Attack1_SubarEath);

		IsAttack = true;

	}
}

//攻撃中
void Player::attack(float delta_time) {
	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && IsJump)
	{
		// ジャンプ開始状態へ

		if (AttackChange) {
			change_state(State::JumpStart, Motion_Jump_SaberEarth);
		}
		else if (!AttackChange) {
			change_state(State::JumpStart, Motion_JumpStart_GunEarth);
		}
		// ジャンプ
		velocity_.y = JumpHight;
		return;
	}

	if (IsAttack)
	{
		generate_bullet();
		IsAttack = false;
	}

	//攻撃モーションの終了を待つ ここの時間によって弾の生成間隔にもなっている
	if (state_timer_ >= 10) {
		move(delta_time);

	}

	//撃っている途中で０になったらステータス移行

	JudgementBullet();

}

//斬撃
void Player::slash(float delta_time) {

	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && IsJump)
	{
		// ジャンプ開始状態へ
		change_state(State::JumpStart, Motion_Jump_SaberEarth, false);
		// ジャンプ
		velocity_.y = JumpHight;
		return;
	}

	if (IsAttack)
	{
		DamageValue = playerstate_->Attack() * 1.5f;

		//斬撃の弾生成
		generate_attack(DamageValue);

		IsAttack = false;
	}

	if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1) && state_timer_ <= 20)
	{
		SecondAttack_ = true;
	}

	if (SecondAttack_ == true && state_timer_ >= 20.0)
	{
		change_state(State::SecondSlash, Motion_Attack2_SubarEath);

		DamageValue = playerstate_->Attack() * 1.7f;

		generate_attack(DamageValue);
		SecondAttack_ = false;
		return;
	}

	//攻撃モーションの終了を待つ
	if (state_timer_ >= mesh_.MotionEndTime()) {
		move(delta_time);

	}

}

//二回目の斬撃
void Player::Secondslash(float delta_time) {

	if (state_timer_ >= mesh_.MotionEndTime()) {
		move(delta_time);
	}

	if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1) && state_timer_ <= 20) {
		ThirdAttack_ = true;
	}

	if (ThirdAttack_ == true && state_timer_ >= 20.0) {

		change_state(State::ThirdSlash, Motion_Attack3_SubarEath);

		DamageValue = playerstate_->Attack() * 2;

		generate_attack(DamageValue);
		ThirdAttack_ = false;
		return;
	}

}

//三回目の斬撃
void Player::Thirdslash(float delta_time) {

	if (state_timer_ >= mesh_.MotionEndTime()) {
		move(delta_time);
		return;
	}

}

//撃っている最中に０になったらアイドル状態に遷移
void Player::JudgementBullet() {

	if (playerState_()->gunstate_() == PlayerState::GunState::Beamlifl
		&& playerState_()->BeamBullet() == 0) {

		if (IsFly) {
			change_state(State::Move, Motion_Idle_GunAir);
		}
		else if (!IsFly) {
			change_state(State::Move, Motion_Idle_GunEarth);
		}
	}

	if (playerState_()->gunstate_() == PlayerState::GunState::BeamMagnumBullet
		&& playerState_()->BeamMagnumBullet() == 0) {

		if (IsFly) {
			change_state(State::Move, Motion_Idle_GunAir);
		}
		else if (!IsFly) {
			change_state(State::Move, Motion_Idle_GunEarth);
		}

	}

	if (playerState_()->gunstate_() == PlayerState::GunState::BazookaBullet
		&& playerState_()->BazookaBullet() == 0) {

		if (IsFly) {
			change_state(State::Move, Motion_Idle_GunAir);
		}
		else if (!IsFly) {
			change_state(State::Move, Motion_Idle_GunEarth);
		}
	}

}

//ダメージ中
void Player::damage(float delta_time) {
	//ノックバックする（ワールド座標系）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//減速させる
	velocity_ -= GSvector3{ velocity_.x,0.f,velocity_.z }*0.5f * delta_time;
	//ダメージモーションの終了を待つ
	if (state_timer_ >= mesh_.MotionEndTime()) {
		move(delta_time);
	}
}

//ジャンプ開始
void Player::jump_start(float delta_time) {

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };
	//WASD移動
	if (gsGetKeyState(GKEY_W))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = walkSpeed * runSpeed;
		else forward_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_S))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = -walkSpeed * runSpeed;
		else forward_speed = -walkSpeed;
	}
	if (gsGetKeyState(GKEY_A))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = walkSpeed * runSpeed;
		else side_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_D))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = -walkSpeed * runSpeed;
		else side_speed = -walkSpeed;
	}

	velocity_.z = forward_speed;
	velocity_.x = side_speed;
	transform_.translate(side_speed * delta_time, 0.f, forward_speed * delta_time);

	if (gsGetKeyTrigger(GKEY_SPACE)) {

		velocity_.y = 0.0f;
		IsFly = true;

		if (AttackChange) {
			change_state(State::JumpEnd, Motion_Jump_SaberEarth);
		}
		else if (!AttackChange) {
			change_state(State::JumpEnd, Motion_JumpEnd_GunEarth);
		}

	}

	if (state_timer_ >= mesh_.MotionEndTime()) {
		// ある程度したら、すぐにジャンプ中モーションへ

		if (AttackChange) {
			change_state(State::JumpEnd, Motion_Idle_SaberEarth);
		}
		else if (!AttackChange) {
			change_state(State::JumpEnd, Motion_Jump_GunEarth);
		}

	}

}

//ジャンプ中
void Player::jump_(float delta_time) {

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };
	//WASD移動
	if (gsGetKeyState(GKEY_W))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = walkSpeed * runSpeed;
		else forward_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_S))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = -walkSpeed * runSpeed;
		else forward_speed = -walkSpeed;
	}
	if (gsGetKeyState(GKEY_A))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = walkSpeed * runSpeed;
		else side_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_D))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = -walkSpeed * runSpeed;
		else side_speed = -walkSpeed;
	}

	velocity_.z = forward_speed;
	velocity_.x = side_speed;
	transform_.translate(side_speed * delta_time, 0.f, forward_speed * delta_time);


	if (gsGetKeyTrigger(GKEY_SPACE)) {

		velocity_.y = 0.0f;
		IsFly = true;

		if (AttackChange) {
			change_state(State::JumpEnd, Motion_Idle_SaberEarth);
		}
		else if (!AttackChange) {
			change_state(State::JumpEnd, Motion_JumpEnd_GunEarth);
		}
	}

}

//ジャンプ終了
void Player::jump_end(float delta_time) {

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };
	//WASD移動
	if (gsGetKeyState(GKEY_W))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = walkSpeed * runSpeed;
		else forward_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_S))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) forward_speed = -walkSpeed * runSpeed;
		else forward_speed = -walkSpeed;
	}
	if (gsGetKeyState(GKEY_A))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = walkSpeed * runSpeed;
		else side_speed = walkSpeed;
	}
	if (gsGetKeyState(GKEY_D))
	{
		if (gsGetKeyState(GKEY_LSHIFT)) side_speed = -walkSpeed * runSpeed;
		else side_speed = -walkSpeed;
	}

	if (state_timer_ >= 7) {

		if (AttackChange) {
			change_state(State::Move, Motion_Idle_SaberEarth);
		}
		else if (!AttackChange) {
			change_state(State::Move, Motion_Idle_GunEarth);
		}

		//移動攻撃で使う
		IsMoveJump = false;

		IsJump = false;
		IsJumpTime = 15.0f;
	}

}

//移動中の射撃
void Player::move_attack(float delta_time) {


	if (IsFly) {
		GSint motion{ Motion_Idle_GunAir };
	}
	else if (!IsFly) {
		GSint motion{ Motion_Idle_GunEarth };
	}

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };

	//WASD移動
	if (gsGetKeyState(GKEY_W))
	{
		forward_speed = walkSpeed;

		if (IsFly) {
			motion_ = Motion_Attack1_GunAir;
		}
		else if (!IsFly) {
			motion_ = Motion_MAttackF_GunEarth;
		}

	}
	if (gsGetKeyState(GKEY_S))
	{
		forward_speed = -walkSpeed;
		if (IsFly) {
			motion_ = Motion_Attack1_GunAir;
		}
		else if (!IsFly) {
			motion_ = Motion_MAttackB_GunEarth;
		}
	}
	if (gsGetKeyState(GKEY_A))
	{
		side_speed = walkSpeed;
		if (IsFly) {
			motion_ = Motion_Attack1_GunAir;
		}
		else if (!IsFly) {
			motion_ = Motion_MAttackL_GunEarth;
		}
	}
	if (gsGetKeyState(GKEY_D))
	{
		side_speed = -walkSpeed;
		if (IsFly) {
			motion_ = Motion_Attack1_GunAir;
		}
		else if (!IsFly) {
			motion_ = Motion_MAttackR_GunEarth;
		}
	}

	transform_.translate(side_speed * delta_time, 0.f, forward_speed * delta_time);

	//立ち止まったら攻撃開始状態へ
	if (forward_speed == 0.0f && side_speed == 0.0f) {
		if (IsFly) {
			change_state(State::ShootAttack, Motion_Attack1_GunAir);
		}
		else if (!IsFly) {
			change_state(State::ShootAttack, Motion_Attack_GunEarth);
		}
	}

	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && IsJump && !IsFly)
	{
		IsMoveJump = true;
		// ジャンプ開始状態へ
		change_state(State::JumpStart, Motion_JumpStart_GunEarth, false);
		// ジャンプ
		velocity_.y = JumpHight;
		return;
	}

	//ある程度立ったら移動状態医へ
	if (state_timer_ >= mesh_.MotionEndTime()) move(delta_time);

}

//移動中の斬撃
void Player::move_slash(float delta_time) {


	GSint motion{ Motion_Idle_SaberEarth };
	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };

	//WASD移動
	if (gsGetKeyState(GKEY_W))
	{
		forward_speed = walkSpeed;
		//motion_ = MotionFire;
	}
	if (gsGetKeyState(GKEY_S))
	{
		forward_speed = -walkSpeed;
		//motion_ = MotionFire;
	}
	if (gsGetKeyState(GKEY_A))
	{
		side_speed = walkSpeed;
		//motion_ = MotionFire;
	}
	if (gsGetKeyState(GKEY_D))
	{
		side_speed = -walkSpeed;
		//motion_ = MotionFire;
	}

	transform_.translate(side_speed * delta_time, 0.f, forward_speed * delta_time);

	//立ち止まったら攻撃開始状態へ
	if (forward_speed == 0.0f && side_speed == 0.0f) change_state(State::SlashAttack, 20);


	//スペースキーでジャンプ
	if (gsGetKeyState(GKEY_SPACE) && IsJump && !IsFly)
	{
		IsMoveJump = true;
		// ジャンプ開始状態へ
		//change_state(State::JumpStart, MotionJump_Start, false);
		// ジャンプ
		velocity_.y = JumpHight;
		return;
	}

	//ある程度立ったら移動状態医へ
	if (state_timer_ >= mesh_.MotionEndTime()) move(delta_time);

}

//飛行
void Player::Fly(float delta_time) {

	FlyPower -= delta_time * 0.1f;

	float UpSpeed{ 0.0f };

	if (gsGetKeyState(GKEY_SPACE)) {
		UpSpeed += walkSpeed;
	}
	else if (gsGetKeyState(GKEY_LCONTROL)) {
		UpSpeed -= walkSpeed;
	}

	transform_.translate(0, UpSpeed * delta_time, 0);

	if (FlyPower <= 0.0f) {
		IsFly = false;
	}

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
	line.end = position + GSvector3{ 0.f,-FootOffset,0.f };
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

			if (AttackChange) {
				change_state(State::JumpEnd, Motion_Idle_SaberEarth);
			}
			else if (!AttackChange) {
				change_state(State::JumpEnd, Motion_JumpEnd_GunEarth);
			}
		}

		if (IsFly) {
			IsFly = false;
		}

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
	
}

//斬撃の生成
void Player::generate_attack(int value) {

	GSvector3 pos = transform_.position() + transform_.forward() * Distance;
	pos.y += Hight;

	//斬撃の生成
	world_->add_actor(new AttackRange{ world_,pos,GSvector3().zero(),value });

}

//モーション中に弾を生成する
void Player::can_bullet() {

	//マウスクリックで射撃
	if (gsGetMouseButtonState(GMOUSE_BUTTON_1) && !AttackChange) {
		generate_bullet();
	}

	//マウスクリックで斬撃
	if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1) && AttackChange) {

		DamageValue = playerstate_->Attack() * 1.5f;

		generate_attack(DamageValue);
	}

}

//アニメーションイベントの設定
void Player::SetAnimationEvent() {

	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackF_GunEarth, CanBullet * 8, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackB_GunEarth, CanBullet * 8, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 3, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 4, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 5, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 6, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 7, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackL_GunEarth, CanBullet * 8, [this] { can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_MAttackR_GunEarth, CanBullet * 8, [this] {can_bullet(); });

	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 2, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 3, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 4, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 5, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 6, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 7, [this] {can_bullet(); });
	mesh_.AddEvent(Motion_Attack1_GunAir, CanBullet * 8, [this] {can_bullet(); });
}
