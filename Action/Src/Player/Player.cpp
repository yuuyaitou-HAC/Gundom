#include "Player/Player.h"
#include "PlayerState.h"
#include "PlayerBullet/PlayerBullet.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"

//モーション番号
enum {
	MotionIdle = 1,  //アイドル
	Motion_Walk_Front = 2,  //前進
	Motion_Walk_Back = 3,  //後退
	Motion_Walk_Left = 4,  //左歩き
	Motion_Walk_Right = 5,  //右歩き
	MotionFire = 11, //射撃
	MotionDamage = 14, //ダメージ
	MotionJump = 17, //ジャンプ
};

//自分の高さ
const float PlayerHeight{ 1.f };
//衝突判定用の半径
const float PlayerRadius{ 0.5f };
//足元のオフセット
const float FootOffset{ 0.1f };
//重力値
const float Gravity{ -0.016f };

const float runSpeed{ 2.0f };

//コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player,Mesh_Player,Mesh_Player,MotionIdle,true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Move },
	state_timer_{ 0.f } {
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

	playerstate_ = new PlayerState();

}

//更新
void Player::update(float delta_time) {

	walkSpeed = playerstate_->MoveSpeed();

	//状態の更新
	update_state(delta_time);
	//重力値を更新
	velocity_.y += Gravity * delta_time;
	//重力を加える
	transform_.translate(0.f, velocity_.y, 0.f);
	//フィールドとの衝突判定
	collide_field();
	//モーションを変更
	mesh_.ChangeMotion(motion_, motion_loop_);
	//メッシュのモーションを更新
	mesh_.Update(delta_time);
	//ワールド変換行列を設定
	mesh_.Transform(transform_.localToWorldMatrix());
}

//描画
void Player::draw()const {
	//メッシュの描画
	mesh_.Draw();
	//武器を描画
	draw_weapon();
	//衝突判定球のデバック表示
	collider().draw();
}

//武器の描画
void Player::draw_weapon()const {
	glPushMatrix();
	//手のボーン(19番目)の位置に武器のメッシュを描画
	glMultMatrixf(mesh_.BoneMatrices(19));
	gsDrawMesh(Mesh_Weapon);
	glPopMatrix();
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
		change_state(State::Damage, MotionDamage, false);
		return;
	}
	//敵と衝突したか？
	if (other.tag() == "EnemyTag") {
		//アクター同士が重ならないように補正する
		collide_actor(other);
	}
}

PlayerState* Player::playerState_() const
{
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
	case Player::State::Attack:
		attack(delta_time);
		break;
	case Player::State::Damage:
		damage(delta_time);
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
	//スペースキーで打つ
	if (gsGetKeyState(GKEY_SPACE)) {
		change_state(State::Attack, MotionFire);
		//弾を生成する
		generate_bullet();
		return;
	}
	GSvector3 velocity{ 0.f,0.f,0.f };
	velocity = velocity.normalized() * walkSpeed * delta_time;

	//何もしなければアイドル状態
	GSint motion{ MotionIdle };
	//移動しているか？
	if (velocity.length() != 0.f) {
		//向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity), 12.0f * delta_time);
		transform_.rotation(rotation);
		//移動中のモーションにする
		motion = Motion_Walk_Front;
	}
	//モーションの変更
	change_state(State::Move, motion);

	//前後移動する時の速さ
	float forward_speed{ 0.f };
	//左右移動するときの速さ
	float side_speed{ 0.f };

	if (gsGetKeyState(GKEY_W))
	{
		if (gsGetKeyState(GKEY_LSHIFT))
		{
			forward_speed = walkSpeed * runSpeed;
		}
		else
		{
			forward_speed = walkSpeed;

			motion = Motion_Walk_Front;
		}
	}
	if (gsGetKeyState(GKEY_S))
	{
		if (gsGetKeyState(GKEY_LSHIFT))
		{
			forward_speed = -walkSpeed * runSpeed;

		}
		else
		{
			forward_speed = -walkSpeed;
			motion = Motion_Walk_Back;
		}
	}
	if (gsGetKeyState(GKEY_A))
	{
		if (gsGetKeyState(GKEY_LSHIFT))
		{
			side_speed = walkSpeed * runSpeed;

		}
		else
		{
			side_speed = walkSpeed;
			motion = Motion_Walk_Left;
		}
	}
	if (gsGetKeyState(GKEY_D))
	{
		if (gsGetKeyState(GKEY_LSHIFT))
		{
			side_speed = -walkSpeed * runSpeed;
		}
		else
		{
			side_speed = -walkSpeed;
			motion = Motion_Walk_Right;
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
}

//攻撃中
void Player::attack(float delta_time) {
	//攻撃モーションの終了を待つ
	if (state_timer_ >= mesh_.MotionEndTime()) {
		move(delta_time);
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
	//弾を生成する場所の距離
	const float GenerateDistance{ 0.5f };
	//生成する位置の高さの補正値
	const float GenerateHeight{ 1.5f };
	//弾の移動スピード
	const float Speed{ 1.f };
	//生成位置の計算
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	//生成位置の高さを補正する
	position.y += GenerateHeight;
	//移動量の計算
	GSvector3 velocity = transform_.forward() * Speed;
	//弾の生成
	world_->add_actor(new PlayerBullet{ world_,position,velocity });
}