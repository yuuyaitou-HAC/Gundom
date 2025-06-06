#include "Bazooka.h"
#include "World\IWorld.h"
#include "Common\Assets.h"
#include "Player/Player.h"
#include "PlayerBullet/bazookaBullet.h"
#include "Scene/Screen.h"

Bazooka::Bazooka(IWorld* world, const GSvector3& position) :
	player_{ nullptr }
{
	//ワールド設定
	world_ = world;

	//タグの設定
	tag_ = "GunTag";

	//名前の設定
	name_ = "Bazooka";

	//座標の初期化
	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//クールタイムの初期化
	coolTime_ = assignmentCoolTime_ = 240.0f;
}

void Bazooka::update(float delta_time) {
	//マガジン数を取得
	magazin_ = player_->playerState_()->bazookaMagazin();

	if (coolTimeTriger_) {

		deltaTimer_ = delta_time;

		Cool();
	}
}

void Bazooka::Fire()
{
	nowMagazine_ = player_->playerState_()->bazookaBullet();

	if (nowMagazine_ > 0) {
		//弾を生成する場所の距離
		const float GenerateDistance{ 0.5f };
		//生成する位置の高さの補正値
		const float GenerateHeight{ 1.5f };
		//弾の移動スピード
		const float Speed{ 1.f };
		//生成位置の計算
		GSvector3 position = player_->transform().position() + player_->transform().forward() * GenerateDistance;
		//生成位置の高さを補正する
		position.y += GenerateHeight;
	
		float x, y, z, dirX, dirY, dirZ;
		gsCalculateRay(screenwidtht / 2, screenheight / 2, &x, &y, &z, &dirX, &dirY, &dirZ);
		GSvector3 generatevelocity;
		GSvector3 direction = (GSvector3{ dirX,dirY,dirZ });
		GSvector3 pos = (GSvector3{ x,y,z });

		generatevelocity = (world_->find_first_intersection(pos, direction) - position).normalized() * Speed;

		world_->add_actor(new BazookaBullet{ world_,position,generatevelocity,player_->playerState_()->attack() });

		player_->playerState_()->setBazookaBullet(-1);
	}

	if (nowMagazine_ == 1) {
		coolTimeTriger_ = true;
	}
}

void Bazooka::Cool() {

	if (magazin_ < 1) {
		coolTimeTriger_ = false;
		return;
	}

	coolTime_ -= deltaTimer_;

	if (coolTime_ <= 0) {
		coolTimeTriger_ = false;
		coolTime_ = assignmentCoolTime_;
		player_->playerState_()->setBazookaBullet(3);
		deltaTimer_ = 0;
		player_->playerState_()->setBazookaMagazin(-1);
	}
}