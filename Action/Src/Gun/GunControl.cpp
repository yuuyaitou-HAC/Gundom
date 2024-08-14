#include "GunControl.h"
#include "World\IWorld.h"
#include "Common\Assets.h"

#include "Gun/BeamGun.h"

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

	bg = new BeamGun(world_,transform_.position());
}

GunControl::~GunControl() {

	//各銃の削除
	delete bg;
}

void GunControl::update(float delta_time) {

	ChangeState();
}

void GunControl::draw() const {

	bg->draw();
}

void GunControl::draw_gui() const {


}

void GunControl::react(Actor& other) {


}


void GunControl::ChangeState(){

	if (gsGetKeyTrigger(GKEY_1)) {
		bullet = Bullet::Beamlifl;
	}
	else if (gsGetKeyTrigger(GKEY_2)) {
		bullet = Bullet::BeamMagnum;
	}
	else if (gsGetKeyTrigger(GKEY_3)) {
		bullet = Bullet::Bazooka;
	}

}

void GunControl::Fire(){

	bg->Fire();


	if (bullet == Bullet::Beamlifl) {
	}
	else if (bullet == Bullet::BeamMagnum) {
	}
	else if (bullet == Bullet::Bazooka) {
	}

}

BeamGun* GunControl::BG() const
{
	return bg;
}
