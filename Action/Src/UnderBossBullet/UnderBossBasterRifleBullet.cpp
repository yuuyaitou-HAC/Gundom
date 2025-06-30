#include "UnderBossBasterRifleBullet.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include"Collision/Line.h"
#include "Common/Assets.h"
#include "GSeffect.h"

UnderBossBasterRiflrBullet::UnderBossBasterRiflrBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {

	world_ = world;

	tag_ = "EnemyBulletTag";

	name_ = "UnderBossBasterRiflrBullet";

	velocity_ = velocity;

	collider_ = BoundingSphere{ 0.5f };

	transform_.position(position);

	m_AttackValue = Damage;

	quatenion_.setLookRotation(velocity);
	transform_.rotation(quatenion_);

	bulletEffect_ = gsPlayEffect(Effect_EnemyBullet, &position);
}

void UnderBossBasterRiflrBullet::update(float delta_time) {

	effectSize_.setScale(bulletEffectScale_);
	//エフェクトに自身のワールド変換行列を設定
	GSmatrix4 world = effectSize_ * transform_.localToWorldMatrix();
	//ワールド変換行列を設定
	gsSetEffectMatrix(bulletEffect_, &world);

	//寿命が尽きたら死亡
	if (lifespanTimer_ <= 0.f) {
		gsStopEffect(bulletEffect_);
		die();
		return;
	}
	//寿命の更新
	lifespanTimer_ -= delta_time;
	//フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		//交点の座標に補正
		transform_.position(intersect);
		gsStopEffect(bulletEffect_);
		//フィールドに衝突したら死亡
		die();
		return;
	}
	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

void UnderBossBasterRiflrBullet::react(Actor& other) {

	if (other.tag() == "PlayerTag") {
		gsStopEffect(bulletEffect_);
		die();
	}
}