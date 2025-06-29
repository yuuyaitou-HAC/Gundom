#include "BeamGun.h"
#include "GunControl.h"
#include "World\IWorld.h"
#include "Scene/Screen.h"
#include "Common\Assets.h"
#include "Player/Player.h"
#include "PlayerBullet/PlayerBullet.h"

const float BGHeight{ 0.0f };
const float BGRadius{ 0.0f };

BeamGun::BeamGun(IWorld* world, const GSvector3& position) :
	player_{ nullptr }
{
	//ワールド設定
	world_ = world;

	//タグの設定
	tag_ = "GunTag";

	//名前の設定
	name_ = "BeamGun";

	//衝突判定球の設定
	collider_ = BoundingSphere{ BGRadius,GSvector3{0.f,BGHeight,0.f} };
	//座標の初期化
	transform_.position(position);

	//null
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	gunControl_ = static_cast<GunControl*>(world_->find_actor("GunControl"));

	//弾の数
	nowMagazine_ = BazookaMagazine_ = player_->player_state()->beamBullet();
}

void BeamGun::update(float delta_time) {

	if (coolTimeTriger_)Cool(delta_time);
}

void BeamGun::Fire() {

	nowMagazine_ = player_->player_state()->beamBullet();

	if (nowMagazine_ > 0) {


		//生成位置の計算
		GSvector3 position = player_->transform().position();

		position.y += GenerateHeight;

		position += player_->transform().forward().normalized() * GenerateDistance;

		float x, y, z, dirX, dirY, dirZ;
		gsCalculateRay(screenwidtht / 2, screenheight / 2, &x, &y, &z, &dirX, &dirY, &dirZ);
		GSvector3 generatevelocity;
		GSvector3 direction = (GSvector3{ dirX,dirY,dirZ });
		GSvector3 pos = (GSvector3{ x,y,z });

		generatevelocity = (world_->find_first_intersection(pos, direction) - position).normalized() * Speed;

		world_->add_actor(new PlayerBullet{ world_,position,generatevelocity,player_->player_state()->attack(),"BeamRifleBullet" });
	}

	if (nowMagazine_ == 1) coolTimeTriger_ = true;
}

void BeamGun::Cool(float delta_time) {

	coolTime_ -= delta_time;

	if (coolTime_ <= 0) {
		coolTimeTriger_ = false;
		coolTime_ = assignmentCoolTime_;
		player_->player_state()->setBeamBullet(BazookaMagazine_);
	}
}