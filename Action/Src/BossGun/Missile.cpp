#include "Missile.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
//#include "DamageRange.h"
#include "Common\Assets.h"
#include "GSeffect.h"
#include "GSmath.h"

Missile::Missile(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) :
	mesh_{ Mesh_MissileBullet,Mesh_MissileBullet ,Mesh_MissileBullet },
	state_{ State::Up } {

	world_ = world;

	//タグ
	tag_ = "EnemyBulletTag";
	//名前
	name_ = "MissileBullet";

	//当たり判定
	collider_ = BoundingSphere{ 0.2f };

	//座標の初期化
	transform_.position(position);

	//寿命
	lifespanTimer_ = 60.0f;

	//ダメージ量
	m_AttackValue = Damage;

	mesh_.Transform(transform_.localToWorldMatrix());

	//プレイヤー取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//着弾目標地点
	targetPoint_ = player_->transform().position() + GSvector3{ (float)gsRand(-randpos_,randpos_),0.0f,(float)gsRand(-randpos_,randpos_) };
	targetPoint_.y = -11.0f;

	//ボス取得
	boss_ = static_cast<Boss*>(world_->find_actor("Boss"));

}

Missile::~Missile() {
	//エフェクト停止
	gsStopEffect(targetPointEffect_);
	gsStopEffect(vernierEffect_);
}

void Missile::update(float delta_time) {



}

void Missile::draw() const {
	mesh_.Draw();
	collider().draw();
}

void Missile::react(Actor& other) {

}
