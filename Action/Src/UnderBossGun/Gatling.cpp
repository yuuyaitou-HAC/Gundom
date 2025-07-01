#include "Gatling.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "UnderBoss/UnderBoss.h"
#include "Player/Player.h"
#include "EnemyBullet/GatlingBullet.h"

Gatling::Gatling(IWorld* world, const GSvector3& position, const UnderBoss* underBoss) {

	world_ = world;

	tag_ = "BossGunTag";

	name_ = "Gatling";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	//プレイヤー取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//生成の問題上ここでボスを取得
	underBoss_ = underBoss;
	//マガジンの中の弾を取得
	nowMagazine_ = assignmentMagazine_ = underBoss_->underBossState_()->GatlingBullet();
}

void Gatling::update(float delta_time) {

	if (coolTimerTrigger_) {
		Cool(delta_time);
	}
}

void Gatling::Fire() {

	nowMagazine_ = underBoss_->underBossState_()->GatlingBullet();

	if (nowMagazine_ > 0) {

		//ボスの座標
		GSvector3 pos = underBoss_->transform().position() + underBoss_->transform().forward();

		//ボスからプレイヤーに向かって弾を撃つ ランダム性込み
		GSvector3 velocity = ((player_->transform().position() - pos) + GSvector3{ gsRandf(randam_.x,randam_.y),gsRandf(randam_.x,randam_.y) ,gsRandf(randam_.x,randam_.y) }).normalized();

		pos.y += 1.5f;

		world_->add_actor(new GatlingBullet{ world_,pos,velocity,attackValue_ });

		underBoss_->underBossState_()->SetGatlingBullet(-1);
	}

	if (nowMagazine_ == 1)coolTimerTrigger_ = true;
}

void Gatling::Cool(float delta_time) {

	coolTimer_ -= delta_time;

	if (coolTimer_ <= 0) {
		coolTimerTrigger_ = false;
		coolTimer_ = assignmentCoolTimer_;
		underBoss_->underBossState_()->SetGatlingBullet(assignmentMagazine_);
	}
}