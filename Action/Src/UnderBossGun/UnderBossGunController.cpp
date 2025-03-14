#include "UnderBossGunController.h"
#include "World/IWorld.h"
#include "Common/Assets.h"

UnderBossGunController::UnderBossGunController(IWorld* world, const GSvector3& position) {

	world_ = world;

	transform_.position(position);

	//各銃の生成
	BR = new UnderBossBeamRifle(world_, transform_.position());
	G = new Gatling(world_, transform_.position());
	BS = new UnderBossBasterRifle(world_, transform_.position());

	//銃のステータスの設定
	GunNum = 1;

}

UnderBossGunController::~UnderBossGunController() {

	//銃管理クラスで生成したものの削除
	delete BR;
	delete G;
	delete BS;
}

void UnderBossGunController::update(float delta_time) {

	//ステータスを変更
	changeState();

	//各銃のアップデートを呼ぶ
	BR->update(delta_time);
	G->update(delta_time);

	//弾発射間隔の更新
	Fire_timer += delta_time;
}

void UnderBossGunController::changeState() {

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

void UnderBossGunController::draw() const
{
	G->draw();
}

void UnderBossGunController::SetState(int num) {

	GunNum = num;

}

void UnderBossGunController::Fire() {

	if (gunstate == GunState::Beamlifl) {// && Fire_timer >=600.0f) {

		BR->fire();
	}
	else if (gunstate == GunState::Gatling && Fire_timer >= 10.0f) {

		G->Fire();

	}
	else if (gunstate == GunState::Basterlifl) {
		BS->fire();
	}

}
