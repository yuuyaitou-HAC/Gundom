#include "Gatling.h"
#include "World/IWorld.h"
#include "Common/Assets.h"
#include "UnderBoss/UnderBoss.h"
#include "Player/Player.h"
#include "EnemyBullet/GatlingBullet.h"


Gatling::Gatling(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "BossGunTag";

	name_ = "Gatling";

	collider_ = BoundingSphere{ 0 };

	transform_.position(position);

	//プレイヤー取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//クールタイムを設定
	coolTimer_ = assignmentCoolTimer_ = 240.0f;

	//ガトリングの弾の拡散範囲
	randam_ = { -2,2 };
}

void Gatling::update(float delta_time) {

	//アップデート時に一回だけ呼ぶ
	if (!oneTrigger_) {

		//生成の問題上ここでボスを取得
		boss_ = static_cast<UnderBoss*>(world_->find_actor("UnderBoss"));
		//マガジンの中の弾を取得
		nowMagazine_ = assignmentMagazine_ = boss_->underBossState_()->GatlingBullet();
		//再び入らないようにフラグを変える
		oneTrigger_ = true;
	}

	if (coolTimerTrigger_) {

		deltaTimer_ = delta_time;

		Cool();
	}
}

void Gatling::Fire() {

	nowMagazine_ = boss_->underBossState_()->GatlingBullet();

	if (nowMagazine_ > 0) {

		//ボスの座標
		GSvector3 pos = boss_->transform().position() + boss_->transform().forward();

		//ボスからプレイヤーに向かって弾を撃つ ランダム性込み
		GSvector3 velocity = ((player_->transform().position() - pos) + GSvector3{ gsRandf(randam_.x,randam_.y),gsRandf(randam_.x,randam_.y) ,gsRandf(randam_.x,randam_.y) }).normalized();

		pos.y += 1.5f;

		world_->add_actor(new GatlingBullet{ world_,pos,velocity,1 });

		boss_->underBossState_()->SetGatlingBullet(-1);
	}

	if (nowMagazine_ == 1)coolTimerTrigger_ = true;
}

void Gatling::Cool() {

	coolTimer_ -= deltaTimer_;

	if (coolTimer_ <= 0) {
		coolTimerTrigger_ = false;
		coolTimer_ = assignmentCoolTimer_;
		boss_->underBossState_()->SetGatlingBullet(assignmentMagazine_);
		deltaTimer_ = 0;
	}
}