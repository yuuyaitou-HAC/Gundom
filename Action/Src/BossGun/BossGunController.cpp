#include "BossGunController.h"
#include "World/IWorld.h"
#include "Common/Assets.h"

BossGunController::BossGunController(IWorld* world, const GSvector3& position) {

	world_ = world;

	transform_.position(position);

	//各銃の生成
	BR = new BossBeamRifle(world_, transform_.position());
	G = new Gatling(world_, transform_.position());

	//銃のステータスの設定
	GunNum = 1;

}

BossGunController::~BossGunController() {

	//銃管理クラスで生成したものの削除
	delete BR;
	delete G;

}

void BossGunController::update(float delta_time) {

	//ステータスを変更
	changeState();

	//各銃のアップデートを呼ぶ
	BR->update(delta_time);
	G->update(delta_time);

	//弾発射間隔の更新
	Fire_timer += delta_time;
}

void BossGunController::changeState() {

	switch (GunNum)
	{
	case 1:
		gunstate = GunState::Beamlifl;
		break;
	case 2:
		gunstate = GunState::Gatling;
		break;
	case 3:
		gunstate = GunState::Basterlifl;
		break;
	}

}

void BossGunController::draw() const
{
	G->draw();
}

void BossGunController::SetState(int num) {

	GunNum = num;

}

void BossGunController::Fire() {

	if (gunstate == GunState::Beamlifl) {// && Fire_timer >=600.0f) {

		BR->Fire();
	}
	else if (gunstate == GunState::Gatling && Fire_timer >= 10.0f) {

		G->Fire();

	}
	else if (gunstate == GunState::Basterlifl && Fire_timer >= 10.0f) {

	}

}
