#include "UnderBossBasterRifle.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "UnderBoss/UnderBoss.h"
#include "Player/Player.h"
#include "UnderBossBullet/UnderBossBasterRifleBullet.h"


UnderBossBasterRifle::UnderBossBasterRifle(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "BossGunTag";

	name_ = "BossBasterRifle";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));
}

void UnderBossBasterRifle::update(float delta_time) {
	if (!onrTrigger_) {
		//生成の問題上ここでボスを取得する
		boss_ = static_cast<UnderBoss*>(world_->find_actor("UnderBoss"));

		//再度はいらないようにフラグを変える
		onrTrigger_ = true;
	}
}

void UnderBossBasterRifle::fire() {

	//弾の生成
	GSvector3 pos = boss_->transform().position() + boss_->transform().forward();

	const float speed{ 0.5f };

	GSvector3 velocity = (player_->transform().position() - pos).normalized() * speed;

	pos.y += 1.5f;

	//弾生成
	world_->add_actor(new UnderBossBasterRiflrBullet{ world_, pos, velocity, 5 });
}
