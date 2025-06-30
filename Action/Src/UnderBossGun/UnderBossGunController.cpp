#include "UnderBossGunController.h"
#include "World/IWorld.h"
#include "Common/Assets.h"

UnderBossGunController::UnderBossGunController(IWorld* world, const GSvector3& position, const UnderBoss* underBoss) {

	world_ = world;

	transform_.position(position);

	//各銃の生成
	beamLifle_ = new UnderBossBeamRifle(world_, transform_.position(), underBoss);
	gatling_ = new Gatling(world_, transform_.position(), underBoss);
	basterLifle_ = new UnderBossBasterRifle(world_, transform_.position(), underBoss);

	//銃のステータスの設定
	gunNum_ = 1;
}

UnderBossGunController::~UnderBossGunController() {

	//銃管理クラスで生成したものの削除
	delete beamLifle_;
	delete gatling_;
	delete basterLifle_;
}

void UnderBossGunController::update(float delta_time) {

	//ステータスを変更
	changeState();

	//各銃のアップデートを呼ぶ
	beamLifle_->update(delta_time);
	gatling_->update(delta_time);

	//弾発射間隔の更新
	fireTimer_ -= delta_time;
}

void UnderBossGunController::changeState() {

	switch (gunNum_)
	{
	case 1:
		gunState_ = GunState::Beamlifl;
		break;
	case 2:
		gunState_ = GunState::Gatling;
		break;
	case 3:
		gunState_ = GunState::Basterlifl;
		break;
	}
}

void UnderBossGunController::draw() const {
	gatling_->draw();
}

void UnderBossGunController::SetState(int num) {
	gunNum_ = num;
}

void UnderBossGunController::Fire() {

	if (gunState_ == GunState::Beamlifl) {
		beamLifle_->fire();
	}
	else if (gunState_ == GunState::Gatling && fireTimer_ <= 0) {
		fireTimer_ = assignmentFireTimer_;
		gatling_->Fire();
	}
	else if (gunState_ == GunState::Basterlifl) {
		basterLifle_->fire();
	}
}
