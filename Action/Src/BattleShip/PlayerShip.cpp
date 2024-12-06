#include "PlayerShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Collision/Ray.h"
#include "Player/Player.h"

const float PlayerShipRadius_{ 0.8f };
const float PlayerShipHeight_{ 1.f };

PlayerShip::PlayerShip(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_PlayerShip,Mesh_PlayerShip ,Mesh_PlayerShip ,0 } {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "PlayerShiip";

	collider_ = BoundingSphere{ PlayerShipRadius_,GSvector3{0,PlayerShipHeight_,0} };

	transform_.position(position);

	//プレイヤー
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//補給ポイント用の当たり判定生成
	cd_ = new CollisionDerection{ world_,GSvector3{118,22,-3},"PlayerTag",1.0f };
	world_->add_actor(cd_);

}

void PlayerShip::update(float delta_time) {

	pos = transform_.position();

	//メッシュを更新
	mesh_.Update(delta_time);

	mesh_.Transform(transform_.localToWorldMatrix());

	if (cd_->Frag()) {
		//補給
		supply();
		
		//フラグの初期化
		cd_->setFrag(false);
	}

}

void PlayerShip::draw() const {
	mesh_.Draw();
	collider().draw();
}

void PlayerShip::react(Actor& other) {

}

void PlayerShip::supply() {

	//レベルアップの処理

	//補給
	player_->playerState_()->supply();

}
