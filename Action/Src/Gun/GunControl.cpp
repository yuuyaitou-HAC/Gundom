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

	drawtest();
}

void GunControl::ChangeState() {

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

void GunControl::Fire() {

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

void GunControl::drawtest() const {

	//gsTextPos(100, 200);
	//gsDrawText("攻撃力 =%d",player->playerState_()->Attack());
	//gsTextPos(100, 250);
	//gsDrawText("防御力=%d", player->playerState_()->Defense());
	//gsTextPos(100, 300);
	//gsDrawText("現在のHP=%d",player->playerState_()->HP());
	//gsTextPos(100, 350);
	//gsDrawText("最大HP=%d", player->playerState_()->MaxHP());
	//gsTextPos(100, 400);
	//gsDrawText("移動速度=%f", player->playerState_()->MoveSpeed());
	////gsTextPos(100, 450);
	////gsDrawText("エネルギー量=%f", player->playerState_()->Enargy());
	//gsTextPos(100, 550);
	//gsDrawText("ビームライフルの弾=%d", player->playerState_()->BeamBullet());
	//gsTextPos(100, 600);
	//gsDrawText("ビームマグナムの弾=%d", player->playerState_()->BeamMagnumBullet());
	//gsTextPos(100,650);
	//gsDrawText("ビームマグナムのマガジン=%d",bm->test());
	//gsTextPos(100, 700);
	//gsDrawText("バズーカの弾=%d", player->playerState_()->BazookaBullet());
	//gsTextPos(100, 750);
	//gsDrawText("バズーカのマガジン=%d",bz->Test());

}
