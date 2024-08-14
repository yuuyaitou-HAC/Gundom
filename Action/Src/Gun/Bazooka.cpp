#include "Bazooka.h"
#include "World\IWorld.h"
#include "Common\Assets.h"
#include "Player/Player.h"
#include "PlayerBullet/bazookaBullet.h"

const float BGHeight{ 0.0f };
const float BGRadius{ 0.0f };

Bazooka::Bazooka(IWorld* world, const GSvector3& position):
	player_{nullptr}
{
	//ワールド設定
	world_ = world;

	//タグの設定
	tag_ = "BazookaTag";

	//名前の設定
	name_ = "Bazooka";

	//衝突判定球の設定
	collider_ = BoundingSphere{ BGRadius,GSvector3{0.f,BGHeight,0.f} };
	//座標の初期化
	transform_.position(position);

	//null
	player_ = static_cast<Player*>(world_->find_actor("Player"));
}

void Bazooka::update(float delta_time)
{
}

void Bazooka::draw() const
{
}

void Bazooka::draw_gui() const
{
}

void Bazooka::react(Actor& other)
{
}

void Bazooka::Fire()
{
	//GStransform playerTrans = player_->transform();

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

	world_->add_actor(new BazookaBullet{ world_,position,velocity,player_->playerState_()->Attack() });
}
