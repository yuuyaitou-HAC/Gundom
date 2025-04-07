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

	guncontrol = static_cast<GunControl*>(world_->find_actor("GunControl"));

	//弾の数
	NowMagazine = AsignmentMagazine = player_->playerState_()->beamBullet();

	//クールタイム　4秒
	CoolTime = AsignmentCoolTime = 240.0f;
}

void BeamGun::update(float delta_time) {

	if (CoolTimeTriger) {
	
		delta_timer = delta_time;

		Cool();
	}
}

void BeamGun::Fire() {

	NowMagazine = player_->playerState_()->beamBullet();

	if (NowMagazine > 0) {

		//弾を生成する場所の距離
		const float GenerateDistance{ 1.8f };
		//生成する位置の高さの補正値 1.5
		const float GenerateHeight{ 1.7f };
		//弾の移動スピード
		const float Speed{ 1.f };
		//生成位置の計算
		GSvector3 position = player_->transform().position();

		position.y += GenerateHeight;

		position += player_->transform().forward().normalized() * GenerateDistance;
	
		float x, y, z, dirX, dirY, dirZ;
		gsCalculateRay(screenwidtht / 2, screenheight / 2, &x, &y, &z, &dirX, &dirY, &dirZ);
		GSvector3 generatevelocity;
		GSvector3 direction = (GSvector3{ dirX,dirY,dirZ });
		GSvector3 pos = (GSvector3{ x,y,z });

		generatevelocity = (world_->find_first_intersection(pos, direction) - position ).normalized() * Speed;
		
		world_->add_actor(new PlayerBullet{ world_,position,generatevelocity,player_->playerState_()->attack(),"BeamRifleBullet"});
	}

	if (NowMagazine == 1) CoolTimeTriger = true;
}

void BeamGun::Cool(){

	CoolTime -= delta_timer;

	if (CoolTime <= 0) {
		CoolTimeTriger = false;
		CoolTime = AsignmentCoolTime;
		player_->playerState_()->setBeamBullet(AsignmentMagazine);
		delta_timer = 0;
	}

}
