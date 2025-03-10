#include "BossBasterRifle.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "UnderBoss/UnderBoss.h"
#include "Player/Player.h"
#include "EnemyBullet/BasterRifleBullet.h"


BossBasterRifle::BossBasterRifle(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "BossGunTag";

	name_ = "BossBasterRifle";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	player = static_cast<Player*>(world_->find_actor("Player"));
}

void BossBasterRifle::update(float delta_time) {
	if (!FarstUpdate_) {
		//生成の問題上ここでボスを取得する
		boss = static_cast<UnderBoss*>(world_->find_actor("UnderBoss"));

		//再度はいらないようにフラグを変える
		FarstUpdate_ = true;
	}
}

void BossBasterRifle::fire() {

	//弾の生成
	GSvector3 pos = boss->transform().position() + boss->transform().forward();

	const float speed{ 0.5f };

	GSvector3 velocity = (player->transform().position() - pos).normalized() * speed;

	pos.y += 1.5f;

	//弾生成
	world_->add_actor(new BasterRiflrBullet{ world_, pos, velocity, 5 });
}
