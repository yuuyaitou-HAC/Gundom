#include "BeamGun.h"
#include "World\IWorld.h"
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
	tag_ = "BeamGunTag";

	//名前の設定
	name_ = "BeamGun";

	//衝突判定球の設定
	collider_ = BoundingSphere{ BGRadius,GSvector3{0.f,BGHeight,0.f} };
	//座標の初期化
	transform_.position(position);

	//null
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//弾の数
	//NowMagazine = player_->playerState_()->BeamBullet();

	//クールタイム　4秒
	CoolTime = AsignmentCoolTime = 240.0f;

}

void BeamGun::update(float delta_time) {

	if (CoolTimeTriger) {
	
		delta_timer = delta_time;

		Cool();
	}

}

void BeamGun::draw() const {



	
}

void BeamGun::draw_gui() const {
}

void BeamGun::react(Actor& other) {

}

void BeamGun::Fire() {

	NowMagazine = player_->playerState_()->BeamBullet();

	if (NowMagazine > 0) {

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
		//移動量の計算
		GSvector3 velocity = player_->transform().forward() * Speed;

		world_->add_actor(new PlayerBullet{ world_,position,velocity,player_->playerState_()->Attack() });

		player_->playerState_()->SetBeamBullet(-1);
	}

	if (NowMagazine == 1) {
		CoolTimeTriger = true;
	}

}

void BeamGun::Cool(){

	CoolTime -= delta_timer;

	if (CoolTime <= 0) {
		CoolTimeTriger = false;
		CoolTime = AsignmentCoolTime;
		player_->playerState_()->SetBeamBullet(10);
		delta_timer = 0;
	}

}
