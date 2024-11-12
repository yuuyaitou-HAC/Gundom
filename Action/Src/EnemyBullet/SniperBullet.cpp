#include "SniperBullet.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

SniperBullet::SniperBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {

	world_ = world;

	tag_ = "EnemyBulletTag";

	name_ = "SniperBullet";

	velocity_ = velocity;

	collider_ = BoundingSphere{ 0.05f };

	transform_.position(position);

	lifespan_timer = 120.0f;

	m_AttackValue = Damage;

}


void SniperBullet::update(float delta_time) {
	//寿命が尽きたら死亡
	if (lifespan_timer <= 0.f) {
		die();
		return;
	}
	//寿命の更新
	lifespan_timer -= delta_time;
	//フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		//交点の座標に補正
		transform_.position(intersect);
		//フィールドに衝突したら死亡
		die();
		return;
	}
	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

}

void SniperBullet::draw() const {

	collider().draw();
}

void SniperBullet::react(Actor& other) {

	if (other.tag() != "EnemyTag") {

		die();
	}

}
