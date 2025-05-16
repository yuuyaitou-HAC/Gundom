#include "PlayerShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Collision/Ray.h"
#include "Player/Player.h"
#include "Common/GameData.h"
#include "GSeffect.h"
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

	////補給ポイント用の当たり判定生成
	//cd_ = new CollisionDerection{ world_,GSvector3{209.7,17,7.7},"PlayerTag",1.0f };
	//world_->add_actor(cd_);

	effectTrigger_ = true;

	//バーニア
	vernier1 = gsPlayEffect(Effect_VernierBL, &position);
	vernier2 = gsPlayEffect(Effect_VernierBL, &position);
	vernier3 = gsPlayEffect(Effect_VernierBL, &position);

}

void PlayerShip::update(float delta_time) {

	myPos_ = transform_.position();

	playerPos_ = player_->transform().position();

	//メッシュを更新
	mesh_.Update(delta_time);

	mesh_.Transform(transform_.localToWorldMatrix());

	////補給とレベルアップの処理
	//if (cd_->Frag()) {

	//	world_->gameData()->setPlayerSupply(true);

	//	//プレイヤーの位置や視点の調整
	//	player_->transform().position(GSvector3{ playerPos_.x,cd_->transform().position().y - 4.5f ,playerPos_.z });
	//	player_->transform().rotate(0, 0, 0);

	//	//フラグの初期化
	//	cd_->setFrag(false);

	//	//補給
	//	supply();
	//}
	//if (delayFrag_)delay(delta_time);

	move(delta_time);

	GSmatrix4 world;
	GSmatrix4 local_matrix;

	//エフェクトの更新
	local_matrix = GSmatrix4::TRS(test1, GSquaternion::euler(GSvector3{ 90,0,0 }), GSvector3{ 3,3,2.5 });
	world = local_matrix * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernier1, &world);

	local_matrix = GSmatrix4::TRS(test2, GSquaternion::euler(GSvector3{ 90,0,0 }), GSvector3{ 3,3,2.5 });
	world = local_matrix * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernier2, &world);

	local_matrix = GSmatrix4::TRS(test3, GSquaternion::euler(GSvector3{ 90,0,0 }), GSvector3{ 3,3,2.5 });
	world = local_matrix * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernier3, &world);

	//それぞれの座標取得
	playerpos_ = player_->transform().position();
	effectpos_ = transform_.position();
	playerpos_.y = effectpos_.y = 0.0f;

	//プレイヤーの距離に応じて描画する
	if (GSvector3::distance(effectpos_, playerpos_) <= 100 && effectTrigger_) {
		//地面の砂埃
		dust = gsPlayEffect(Effect_dust, &myPos_);
		effectTrigger_ = false;
	}
	if (GSvector3::distance(effectpos_, playerpos_) > 100) {
		gsStopEffect(dust);

		effectTrigger_ = true;
	}
	GSvector3 pos = transform_.position();
	pos.y = -8;
	local_matrix = GSmatrix4::TRS(pos, GSquaternion::euler(GSvector3{ 0,0,0 }), GSvector3{ 20,20,20 });
	gsSetEffectMatrix(dust, &local_matrix);

	//色の変更
	GScolor4 FootFasteffectColor = GScolor4(0.6, 0.6, 0.6, 1);
	gsSetEffectColor(dust, &FootFasteffectColor);

}

void PlayerShip::draw() const {
	mesh_.Draw();
}

void PlayerShip::move(float delta_time) {
	timeElapsed_ += delta_time;

	// y軸方向にsinカーブで上下する値を生成
	float offsetY = std::sin(timeElapsed_ * frequency * 3.14159f) * amplitude;

	// 現在の高さに加算して位置を更新
	GSvector3 position = basePosition_;  // 移動の基準位置
	position.y += offsetY;

	transform_.translate(position * delta_time);
}

////補給とレベルアップ
//void PlayerShip::supply() {
//
//	//レベルアップの処理
//
//
//	//補給
//	player_->playerState_()->supply();
//
//	delayFrag_ = true;
//}
//
//void PlayerShip::delay(float delta_time) {
//
//	delayTimer_ -= delta_time;
//
//	if (delayTimer_ <= 0) {
//
//		GSvector3 pos = cd_->transform().position();
//
//		GSvector3 shippos = transform_.position();
//		Ray ray = { shippos,-(transform_.up()) };
//		GSvector3 intersect;
//		world_->field()->collide(ray, shippos.y + 30.0f, &intersect);
//
//		pos.y = intersect.y;
//
//		player_->transform().position(pos);
//
//		//補給終了を知らせる
//		world_->gameData()->setPlayerSupply(false);
//
//		delayTimer_ = assignmentDelayTimer_;
//		delayFrag_ = false;
//	}
//}