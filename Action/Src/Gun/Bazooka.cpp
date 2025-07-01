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
}

void Bazooka::update(float delta_time) {
	//マガジン数を取得
	magazin_ = player_->player_state()->bazookaMagazin();

	if (coolTimeTriger_) {

		Cool(delta_time);
	}
}

void Bazooka::Fire()
{
	nowMagazine_ = player_->player_state()->bazookaBullet();

	if (nowMagazine_ > 0) {

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

		int attackValue_ = player_->player_state()->attack() * 1.5f;

		world_->add_actor(new BazookaBullet{ world_,position,generatevelocity,attackValue_ });

		player_->player_state()->setBazookaBullet(-1);
	}

	if (nowMagazine_ == 1) {
		coolTimeTriger_ = true;
	}
}

void Bazooka::Cool(float delta_time) {

	if (magazin_ < 1) {
		coolTimeTriger_ = false;
		return;
	}

	coolTime_ -= delta_time;

	if (coolTime_ <= 0) {
		coolTimeTriger_ = false;
		coolTime_ = assignmentCoolTime_;
		player_->player_state()->setBazookaBullet(3);
		player_->player_state()->setBazookaMagazin(-1);
	}
}