#include "PlayerShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Collision/Ray.h"
#include "Player/Player.h"
#include "Common/GameData.h"

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
	cd_ = new CollisionDerection{ world_,GSvector3{209.7,17,7.7},"PlayerTag",1.0f };
	world_->add_actor(cd_);

}

void PlayerShip::update(float delta_time) {

	pos = transform_.position();

	playerPos_ = player_->transform().position();

	//メッシュを更新
	mesh_.Update(delta_time);

	mesh_.Transform(transform_.localToWorldMatrix());

	//補給とレベルアップの処理
	if (cd_->Frag()) {

		world_->gameData()->setPlayerSupply(true);

		//プレイヤーの位置や視点の調整
		player_->transform().position(GSvector3{ playerPos_.x,17,playerPos_.z });
		player_->transform().rotate(0, 0, 0);

		//フラグの初期化
		cd_->setFrag(false);

		//補給
		supply();
	}
	if (delayFrag_)delay(delta_time);
}

void PlayerShip::draw() const {
	mesh_.Draw();
	collider().draw();

	Actor* player = cd_->actor();

	if (player != NULL) {
		if (player->tag() == "PlayerTag") {
			gsTextPos(100, 400);
			gsDrawText("対象");
		}
	}


}

void PlayerShip::react(Actor& other) {

}

//補給とレベルアップ
void PlayerShip::supply() {

	//レベルアップの処理


	//補給
	player_->playerState_()->supply();

	delayFrag_ = true;
}

void PlayerShip::delay(float delta_time) {

	delayTimer_ -= delta_time;

	if (delayTimer_ <= 0) {
		//補給終了を知らせる
		world_->gameData()->setPlayerSupply(false);

		delayTimer_ = assignmentDelayTimer_;
		delayFrag_ = false;
	}
}
