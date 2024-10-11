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

	//ボス取得
	boss = static_cast<Boss*>(world_->find_actor("Boss"));

	//マガジン内の弾設定
	NowMagazine = AsignmentMagazin = boss->bossState_()->BeamBullet();

	//クールタイムの設定
	CoolTimer = AsignmentCoolTimer = 120.0f;

}

void BossBeamRifle::update(float delta_time) {

	if (CoolTimerTriger) {

		delta_time = delta_time;

		Cool();
	}

}

void BossBeamRifle::Fire() {

	NowMagazine = boss->bossState_()->BasterBullet();

	if (NowMagazine > 0) {
		//弾の生成

		//ボスの座標
		GSvector3 pos = boss->transform().position();
		pos.y += 1.0f;

		//プレイヤーの座標
		GSvector3 Playerpos = player->transform().position();
		Playerpos.y += 1.0f;

		//ボスからプレイヤーに向かって弾を撃つ
		GSvector3 velocity = (Playerpos - pos).normalized();

		//ボスの前方向に飛ばす
		//GSvector3 velocity = boss->transform().forward();

		//弾生成
		world_->add_actor(new BossBeamRifleBullet{ world_,pos,velocity,5 });

	}

	if (NowMagazine == 1) {
		CoolTimerTriger = true;
	}

}

void BossBeamRifle::Cool() {

	CoolTimer -= delta_time;

	if (CoolTimer <= 0) {
		CoolTimerTriger = false;
		CoolTimer = AsignmentCoolTimer;
		boss->bossState_()->SetBeamBullet(AsignmentMagazin);
		delta_time = 0;
	}
}
