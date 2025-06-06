#include "GunControl.h"
#include "World\IWorld.h"
#include "Common\Assets.h"
#include "Gun/BeamGun.h"
#include "Player/Player.h"

const float BGHeight{ 0.0f };
const float BGRadius{ 0.0f };

GunControl::GunControl(IWorld* world, const GSvector3& position) {

	//ワールド設定
	world_ = world;

	//タグの設定
	tag_ = "GunTag";

	//名前の設定
	name_ = "GunControl";

	//衝突判定球の設定
	collider_ = BoundingSphere{ BGRadius,GSvector3{0.f,BGHeight,0.f} };
	//座標の初期化
	transform_.position(position);

	bullet_ = Bullet::Beamlifl;

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	bg_ = new BeamGun(world_, transform_.position());

	bm_ = new BeamMagnum(world_, transform_.position());

	bz_ = new Bazooka(world_, transform_.position());
}

GunControl::~GunControl() {
	//各銃の削除
	delete bg_;
	delete bm_;
	delete bz_;
}

void GunControl::update(float delta_time) {
	ChangeState(delta_time);
	bg_->update(delta_time);
	bm_->update(delta_time);
	bz_->update(delta_time);
}

void GunControl::ChangeState(float delta_time) {

	//マウスホイールの動きを-1～1の間で取得
	gsGetMouseVelocity(0, 0, &mouseZ_);
	mouseZ_ = CLAMP(mouseZ_, -1, 1);

	// クールタイマーが経過していない間は武器変更不可
	if (changeCollTimer_ > 0.0f) {
		changeCollTimer_ -= delta_time;
	}

	// クールタイムが終了していて、ホイールが動いたときだけ変更
	if (mouseZ_ != 0 && changeCollTimer_ <= 0.0f) {
		stateNum_ -= mouseZ_;

		if (stateNum_ > 2)stateNum_ = 0;
		if (stateNum_ < 0)stateNum_ = 2;

		// タイマー開始
		changeCollTimer_ = assignmentChangeCollTimer_;
	}

	if (nowNum_ != stateNum_) {

		switch (stateNum_)
		{
		case 0:
			player_->playerState_()->setGunState(PlayerState::GunState::Beamlifl);
			break;
		case 1:
			player_->playerState_()->setGunState(PlayerState::GunState::BeamMagnumBullet);
			break;
		case 2:
			player_->playerState_()->setGunState(PlayerState::GunState::BazookaBullet);
			break;
		}
		nowNum_ = stateNum_;
	}

	if (gsGetKeyTrigger(GKEY_1)) {
		player_->playerState_()->setGunState(PlayerState::GunState::Beamlifl);
	}
	else if (gsGetKeyTrigger(GKEY_2)) {
		player_->playerState_()->setGunState(PlayerState::GunState::BeamMagnumBullet);
	}
	else if (gsGetKeyTrigger(GKEY_3)) {
		player_->playerState_()->setGunState(PlayerState::GunState::BazookaBullet);
	}
}

void GunControl::Fire() {

	if (player_->playerState_()->gunstate_() == PlayerState::GunState::Beamlifl) {
		gsPlaySE(SE_BeamLifle);
		bg_->Fire();
	}
	else if (player_->playerState_()->gunstate_() == PlayerState::GunState::BeamMagnumBullet) {
		gsPlaySE(SE_BeamMagnum);
		bm_->Fire();
	}
	else if (player_->playerState_()->gunstate_() == PlayerState::GunState::BazookaBullet) {
		gsPlaySE(SE_Bazooca);
		bz_->Fire();
	}
}