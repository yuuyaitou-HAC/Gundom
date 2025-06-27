#include "PlayerShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "Collision/Ray.h"
#include "Player/Player.h"
#include "GSeffect.h"
#include "GSmath.h"

PlayerShip::PlayerShip(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_PlayerShip,Mesh_PlayerShip ,Mesh_PlayerShip ,0 } {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "PlayerShiip";

	collider_ = BoundingSphere{ PlayerShipRadius_,GSvector3{0,PlayerShipHeight_,0} };

	transform_.position(position);

	//プレイヤー
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//バーニア
	vernierEffect1_ = gsPlayEffect(Effect_VernierBL, &position);
	vernierEffect2_ = gsPlayEffect(Effect_VernierBL, &position);
	vernierEffect3_ = gsPlayEffect(Effect_VernierBL, &position);
}

//更新
void PlayerShip::update(float delta_time) {

	myPos_ = transform_.position();

	playerPos_ = player_->transform().position();

	//メッシュを更新
	mesh_.Update(delta_time);

	mesh_.Transform(transform_.localToWorldMatrix());

	//移動
	move(delta_time);

	//エフェクトの更新
	effect_update();
}

//描画
void PlayerShip::draw() const {
	mesh_.Draw();
}

//エフェクトの更新
void PlayerShip::effect_update() {
	//エフェクトの更新
	localMatrix_ = GSmatrix4::TRS(vernierEffectPos1_, GSquaternion::euler(vernierEffectEuler_), vernierEffectScale_);
	effectWorld_ = localMatrix_ * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernierEffect1_, &effectWorld_);

	localMatrix_ = GSmatrix4::TRS(vernierEffectPos2_, GSquaternion::euler(vernierEffectEuler_), vernierEffectScale_);
	effectWorld_ = localMatrix_ * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernierEffect2_, &effectWorld_);

	localMatrix_ = GSmatrix4::TRS(vernierEffectPos3_, GSquaternion::euler(vernierEffectEuler_), vernierEffectScale_);
	effectWorld_ = localMatrix_ * transform_.localToWorldMatrix();
	gsSetEffectMatrix(vernierEffect3_, &effectWorld_);

	//それぞれの座標取得
	playerPos_ = player_->transform().position();
	effectPos_ = myPos_;
	playerPos_.y = effectPos_.y = 0.0f;

	//プレイヤーの距離に応じて描画する
	if (GSvector3::distance(effectPos_, playerPos_) <= effectDrawDistance_) {

		if (effectDrawTrigger_) {
			//地面の砂埃
			dustEffect_ = gsPlayEffect(Effect_dust, &myPos_);
			effectDrawTrigger_ = false;
		}
	}
	else {
		gsStopEffect(dustEffect_);

		effectDrawTrigger_ = true;
	}
	dustEffectPos_ = myPos_;
	dustEffectPos_.y = dustEffectposY_;
	localMatrix_ = GSmatrix4::TRS(dustEffectPos_, GSquaternion::euler(dustEffectEuler_), dustEffectScale_);
	gsSetEffectMatrix(dustEffect_, &localMatrix_);

	//色の変更
	gsSetEffectColor(dustEffect_, &dustEffectColor_);
}

//移動
void PlayerShip::move(float delta_time) {
	timeElapsed_ += delta_time;

	// y軸方向にsinカーブで上下する値を生成
	float offsetY = std::sin(timeElapsed_ * frequency_ * GS_PI) * amplitude_;

	// 現在の高さに加算して位置を更新
	GSvector3 position = GSvector3().zero();  // 移動の基準位置
	position.y += offsetY;

	transform_.translate(position * delta_time);
}