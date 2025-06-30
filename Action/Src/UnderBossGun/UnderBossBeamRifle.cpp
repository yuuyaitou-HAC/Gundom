#include "UnderBossBeamRifle.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "UnderBoss/UnderBoss.h"
#include "Player/Player.h"
#include "UnderBossBullet/UnderBossBeamRifleBullet.h"


UnderBossBeamRifle::UnderBossBeamRifle(IWorld* world, const GSvector3& position, const UnderBoss* underBoss) {

	world_ = world;

	tag_ = "UnderBossGunTag";

	name_ = "UnderBossBeamRifle";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	player = static_cast<Player*>(world_->find_actor("Player"));

	//¶¬‚Ì–â‘èã‚±‚±‚Åƒ{ƒX‚ðŽæ“¾‚·‚é
	underBoss_ = underBoss;
	//ƒ}ƒKƒWƒ““à‚Ì’eÝ’è
	nowMagazine_ = assignmentMagazine_ = underBoss_->underBossState_()->BeamBullet();

}

void UnderBossBeamRifle::update(float delta_time) {
	if (coolTimerTrigger_)Cool(delta_time);
}

void UnderBossBeamRifle::fire() {

	nowMagazine_ = underBoss_->underBossState_()->BeamBullet();

	if (nowMagazine_ > 0) {
		//’e‚Ì¶¬
		GSvector3 pos = underBoss_->transform().position() + underBoss_->transform().forward();

		const float Speed{ 0.5f };

		GSvector3 velocity = (player->transform().position() - pos).normalized() * Speed;

		pos.y += makeposOffset_;

		//’e¶¬
		world_->add_actor(new UnderBossBeamRifleBullet{ world_,pos,velocity,attackValur_ });
	}

	if (nowMagazine_ == 1) {
		coolTimerTrigger_ = true;
	}
}

void UnderBossBeamRifle::Cool(float delta_time) {

	coolTimer_ -= delta_time;

	if (coolTimer_ <= 0) {
		coolTimerTrigger_ = false;
		coolTimer_ = assignmentCoolTimer_;
		underBoss_->underBossState_()->SetBeamBullet(assignmentMagazine_);
	}
}