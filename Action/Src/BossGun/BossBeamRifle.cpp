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

	player = static_cast<Player*>(world_->find_actor("Player"));

	//クールタイムの設定
	CoolTimer = AsignmentCoolTimer = 120.0f;

}

void BossBeamRifle::update(float delta_time) {

	if (!a) {
		//生成の問題上ここでボスを取得する
		boss = static_cast<Boss*>(world_->find_actor("Boss"));
		//マガジン内の弾設定
		NowMagazine = AsignmentMagazine = boss->bossState_()->BeamBullet();
		//再度はいらないようにフラグを変える
		a = true;

	}


	if (CoolTimerTrigger) {

		delta_time = delta_time;

		Cool();
	}

}

void BossBeamRifle::Fire() {

	NowMagazine = boss->bossState_()->BasterBullet();

	if (NowMagazine > 0) {
		//弾の生成
		GSvector3 pos = boss->transform().position() + boss->transform().forward();

		const float Speed{ 0.5f };

		GSvector3 velocity = (player->transform().position() - pos).normalized() * Speed;

		pos.y += 1.5f;

		//弾生成
		world_->add_actor(new BossBeamRifleBullet{ world_,pos,velocity,5 });

		//弾の数を減らす
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
