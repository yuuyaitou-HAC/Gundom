#include "BasterRifleBullet.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include"Collision/Line.h"
#include "Common/Assets.h"
#include "GSeffect.h"

BasterRiflrBullet::BasterRiflrBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {

	world_ = world;

	tag_ = "EnemyBulletTag";

	name_ = "BasterRiflrBullet";

	velocity_ = velocity;

	collider_ = BoundingSphere{ 0.5f };

	transform_.position(position);

	lifespan_timer_ = 60.0f;

	m_AttackValue = Damage;

	quatenion.setLookRotation(velocity);
	transform_.rotation(quatenion);

	effect_handle = gsPlayEffect(Effect_EnemyBullet, &position);
}

void BasterRiflrBullet::update(float delta_time) {

	//エフェクトのサイズの調整
	GSmatrix4 effectsize;
	effectsize.setScale(GSvector3{ 2.0f,2.0f,2.0f });
	//エフェクトに自身のワールド変換行列を設定
	GSmatrix4 world = effectsize * transform_.localToWorldMatrix();
	//ワールド変換行列を設定
	gsSetEffectMatrix(effect_handle, &world);

	//寿命が尽きたら死亡
	if (lifespan_timer_ <= 0.f) {
		gsStopEffect(effect_handle);
		die();
		return;
	}
	//寿命の更新
	lifespan_timer_ -= delta_time;
	//フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		//交点の座標に補正
		transform_.position(intersect);
		gsStopEffect(effect_handle);
		//フィールドに衝突したら死亡
		die();
		return;
	}
	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

}

void BasterRiflrBullet::draw() const {
}

void BasterRiflrBullet::react(Actor& other) {

	if (other.tag() == "PlayerTag") {
		gsStopEffect(effect_handle);
		die();
	}
}