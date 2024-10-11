#include "BossGunController.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "BOSS/Boss.h"

BossGunController::BossGunController(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "GunControllerTag";
	name_ = "BossGunController";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	boss = static_cast<Boss*>(world_->find_actor("Boss"));

	BR = new BossBeamRifle(world_, transform_.position());
	G = new Gatling(world_, transform_.position());

}

BossGunController::~BossGunController() {

	delete BR;
	delete G;

}

void BossGunController::update(float delta_time) {

	changeState();

	BR->update(delta_time);
	G->update(delta_time);

}

void BossGunController::changeState() {

	switch (GunNum)
	{
	case 1:
		boss->bossState_()->SetGunState(BossState::GunState::Beamlifl);
		break;
	case 2:
		boss->bossState_()->SetGunState(BossState::GunState::Gatling);
		break;
	case 3:
		boss->bossState_()->SetGunState(BossState::GunState::Basterlifl);
		break;
	}

}

void BossGunController::SetState(int num) {

	GunNum = num;

}

void BossGunController::Fire() {
	if (boss->bossState_()->gunstate_() == BossState::GunState::Beamlifl) {

		BR->Fire();
	}
	else if (boss->bossState_()->gunstate_() == BossState::GunState::Gatling) {

		G->Fire();

	}
	else if (boss->bossState_()->gunstate_() == BossState::GunState::Basterlifl) {

	}

}
