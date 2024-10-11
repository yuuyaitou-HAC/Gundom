#include "Gatling.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "BOSS/Boss.h"
#include "Player/Player.h"
#include "EnemyBullet/GatlingBullet.h"


Gatling::Gatling(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "BossGun";

	name_ = "Gatling";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	//ボス取得
	boss = static_cast<Boss*>(world_->find_actor("Boss"));

	//プレイヤー取得
	player - static_cast<Player*>(world_->find_actor("Player"));

	//現在のマガジンを設定
	NowMagazine = AsignmentMagazine = boss->bossState_()->GatlingBullet();

	//クールタイムを設定
	CoolTimer = AsignmentCoolTimer = 120.0f;

	randam = { 1,10 };

}

void Gatling::update(float delta_time) {

	if (CoolTimerTrigger) {

		delta_time = delta_time;

		Cool();
	}

}

void Gatling::Fire() {

	NowMagazine = boss->bossState_()->BasterBullet();

	if (NowMagazine > 0) {

		//ボスの座標
		GSvector3 pos = boss->transform().position();
		pos.y += 1.0f;

		//プレイヤーの座標
		GSvector3 Playerpos = player->transform().position();
		Playerpos.y += 1.0f;

		//ボスからプレイヤーに向かって弾を撃つ ランダム性込み
		GSvector3 velocity = ((Playerpos - pos) + GSvector3{ gsRandf(randam.x,randam.y),gsRandf(randam.x,randam.y) ,gsRandf(randam.x,randam.y) }).normalized();

		world_->add_actor(new GatlingBullet{ world_,pos,velocity,2 });

		boss->bossState_()->SetGatlingBullet(-1);

	}

	if (NowMagazine == 1) {
		CoolTimerTrigger = true;
	}

}

void Gatling::Cool() {

	CoolTimer -= delta_time;

	if (CoolTimer <= 0) {
		CoolTimerTrigger = false;
		CoolTimer = AsignmentCoolTimer;
		boss->bossState_()->SetBeamBullet(AsignmentMagazine);
		delta_time = 0;
	}

}
