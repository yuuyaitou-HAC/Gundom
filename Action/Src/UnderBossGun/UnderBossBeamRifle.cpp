#include "UnderBossBeamRifle.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "UnderBoss/UnderBoss.h"
#include "Player/Player.h"
#include "UnderBossBullet/UnderBossBeamRifleBullet.h"


UnderBossBeamRifle::UnderBossBeamRifle(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "UnderBossGunTag";

	name_ = "UnderBossBeamRifle";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	player = static_cast<Player*>(world_->find_actor("Player"));

	//クールタイムの設定
	CoolTimer_ = AsignmentCoolTimer_ = 120.0f;

}

void UnderBossBeamRifle::update(float delta_time) {

	if (!FarstUpdate_) {
		//生成の問題上ここでボスを取得する
		boss = static_cast<UnderBoss*>(world_->find_actor("UnderBoss"));
		//マガジン内の弾設定
		NowMagazine_ = AsignmentMagazine_ = boss->underBossState_()->BeamBullet();
		//再度はいらないようにフラグを変える
		FarstUpdate_ = true;
	}

	if (CoolTimerTrigger_)Cool(delta_time);
}

void UnderBossBeamRifle::fire() {

	NowMagazine_ = boss->underBossState_()->BeamBullet();

	if (NowMagazine_ > 0) {
		//弾の生成
		GSvector3 pos = boss->transform().position() + boss->transform().forward();

		const float Speed{ 0.5f };

		GSvector3 velocity = (player->transform().position() - pos).normalized() * Speed;

		pos.y += 1.5f;

		//弾生成
		world_->add_actor(new UnderBossBeamRifleBullet{ world_,pos,velocity,5 });

		//弾の数を減らす
		//boss->bossState_()->SetBeamBullet(-1);
	}

	if (NowMagazine_ == 1) {
		CoolTimerTrigger_ = true;
	}
}

void UnderBossBeamRifle::Cool(float delta_time) {

	CoolTimer_ -= delta_time;

	if (CoolTimer_ <= 0) {
		CoolTimerTrigger_ = false;
		CoolTimer_ = AsignmentCoolTimer_;
		boss->underBossState_()->SetBeamBullet(AsignmentMagazine_);
	}
}