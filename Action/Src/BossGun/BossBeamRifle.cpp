#include "BossBeamRifle.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "BOSS/Boss.h"
#include "Player/Player.h"
#include "EnemyBullet/BossBeamRifleBullet.h"


BossBeamRifle::BossBeamRifle(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "BossGun";

	name_ = "BossBeamRifle";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	//ƒ{ƒXŽæ“¾
	boss = static_cast<Boss*>(world_->find_actor("Boss"));

	player = static_cast<Player*>(world_->find_actor("Player"));

	//ƒ}ƒKƒWƒ““à‚Ì’eÝ’è
	NowMagazine = AsignmentMagazine = boss->bossState_()->BeamBullet();

	//ƒN[ƒ‹ƒ^ƒCƒ€‚ÌÝ’è
	CoolTimer = AsignmentCoolTimer = 120.0f;

}

void BossBeamRifle::update(float delta_time) {

	if (CoolTimerTrigger) {

		delta_time = delta_time;

		Cool();
	}

}

void BossBeamRifle::Fire() {

	NowMagazine = boss->bossState_()->BasterBullet();

	if (NowMagazine > 0) {
		//’e‚Ì¶¬
		GSvector3 pos = boss->transform().position() + boss->transform().forward();

		const float Speed{ 0.5f };

		GSvector3 velocity = (player->transform().position() - pos).normalized() * Speed;

		pos.y += 1.5f;

		//’e¶¬
		world_->add_actor(new BossBeamRifleBullet{ world_,pos,velocity,5 });

		//’e‚Ì”‚ðŒ¸‚ç‚·
		//boss->bossState_()->SetBeamBullet(-1);

	}

	if (NowMagazine == 1) {
		CoolTimerTrigger = true;
	}

}

void BossBeamRifle::Cool() {

	CoolTimer -= delta_time;

	if (CoolTimer <= 0) {
		CoolTimerTrigger = false;
		CoolTimer = AsignmentCoolTimer;
		boss->bossState_()->SetBeamBullet(AsignmentMagazine);
		delta_time = 0;
	}
}
