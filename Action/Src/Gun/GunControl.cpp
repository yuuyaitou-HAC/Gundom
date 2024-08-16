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
	tag_ = "GunControlTag";

	//名前の設定
	name_ = "GunControl";

	//衝突判定球の設定
	collider_ = BoundingSphere{ BGRadius,GSvector3{0.f,BGHeight,0.f} };
	//座標の初期化
	transform_.position(position);

	bullet = Bullet::Beamlifl;

	player = static_cast<Player*>(world_->find_actor("Player"));

	bg = new BeamGun(world_, transform_.position());

	bm = new BeamMagnum(world_, transform_.position());

	bz = new Bazooka(world_, transform_.position());
}

GunControl::~GunControl() {

	//各銃の削除
	delete bg;
	delete bm;
	delete bz;
}

void GunControl::update(float delta_time) {

	ChangeState();

	bg->update(delta_time);
	bm->update(delta_time);
	bz->update(delta_time);
}
void GunControl::draw() const {

	bg->draw();
	bm->draw();
	bz->draw();
}

void GunControl::draw_gui() const {


}

void GunControl::react(Actor& other) {


}


void GunControl::ChangeState(){

	if (gsGetKeyTrigger(GKEY_1)) {
		player->playerState_()->SetGunState(PlayerState::GunState::Beamlifl);
	}
	else if (gsGetKeyTrigger(GKEY_2)) {
		player->playerState_()->SetGunState(PlayerState::GunState::BeamMagnumBullet);
	}
	else if (gsGetKeyTrigger(GKEY_3)) {
		player->playerState_()->SetGunState(PlayerState::GunState::BazookaBullet);
	}



}

void GunControl::Fire(){

	if (player->playerState_()->gunstate_() == PlayerState::GunState::Beamlifl) {
		bg->Fire();
	}
	else if (player->playerState_()->gunstate_() == PlayerState::GunState::BeamMagnumBullet) {
		bm->Fire();
	}
	else if (player->playerState_()->gunstate_() == PlayerState::GunState::BazookaBullet) {
		bz->Fire();
	}

}