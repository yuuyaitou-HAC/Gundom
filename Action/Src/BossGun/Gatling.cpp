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

	//プレイヤー取得
	player = static_cast<Player*>(world_->find_actor("Player"));

	//クールタイムを設定
	CoolTimer = AsignmentCoolTimer = 120.0f;

	//ガトリングの弾の拡散範囲
	randam = { -2,2 };

}

void Gatling::update(float delta_time) {

	//アップデート時に一回だけ呼ぶ
	if (!a) {

		//生成の問題上ここでボスを取得
		boss = static_cast<Boss*>(world_->find_actor("Boss"));
		//マガジンの中の弾を取得
		NowMagazine = AsignmentMagazine = boss->bossState_()->GatlingBullet();
		//再び入らないようにフラグを変える
		a = true;
	}


	if (CoolTimerTrigger) {

		delta_timer = delta_time;

		Cool();
	}

}

void Gatling::draw() const {

	//ボスがNULLでなければ表示する
	if (boss != NULL) {
		gsTextPos(100, 100);
		gsDrawText("弾数　=%d", boss->bossState_()->GatlingBullet());

	}

}

void Gatling::Fire() {

	NowMagazine = boss->bossState_()->GatlingBullet();

	if (NowMagazine > 0) {

		//ボスの座標
		GSvector3 pos = boss->transform().position() + boss->transform().forward();
		

		//ボスからプレイヤーに向かって弾を撃つ ランダム性込み
		GSvector3 velocity = ((player->transform().position() - pos) + GSvector3{ gsRandf(randam.x,randam.y),gsRandf(randam.x,randam.y) ,gsRandf(randam.x,randam.y) }).normalized();

		pos.y += 1.5f;

		world_->add_actor(new GatlingBullet{ world_,pos,velocity,1 });

		boss->bossState_()->SetGatlingBullet(-1);

	}

	if (NowMagazine == 1) {
		CoolTimerTrigger = true;
	}

}

void Gatling::Cool() {

	CoolTimer -= delta_timer;

	if (CoolTimer <= 0) {
		CoolTimerTrigger = false;
		CoolTimer = AsignmentCoolTimer;
		boss->bossState_()->SetBeamBullet(AsignmentMagazine);
		delta_timer = 0;
	}

}
