#include "UnderBossBasterRifle.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "UnderBoss/UnderBoss.h"
#include "Player/Player.h"
#include "UnderBossBullet/UnderBossBasterRifleBullet.h"


UnderBossBasterRifle::UnderBossBasterRifle(IWorld* world, const GSvector3& position, const UnderBoss* underBoss) {

	world_ = world;

	tag_ = "BossGunTag";

	name_ = "BossBasterRifle";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//¶¬‚Ì–â‘èã‚±‚±‚Åƒ{ƒX‚ðŽæ“¾‚·‚é
	underBoss_ = underBoss;
}

void UnderBossBasterRifle::update(float delta_time) {
}

void UnderBossBasterRifle::fire() {

	//’e‚Ì¶¬
	GSvector3 makePos = underBoss_->transform().position() + underBoss_->transform().forward();

	GSvector3 velocity = (player_->transform().position() - makePos).normalized() * speed_;

	makePos.y += makePosOffset_;

	//’e¶¬
	world_->add_actor(new UnderBossBasterRiflrBullet{ world_, makePos, velocity, attackValue_ });
}
