#include "GatlingBullet.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "GSeffect.h"

GatlingBullet::GatlingBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) :
	mesh_{ Mesh_GatringBullet,Mesh_GatringBullet ,Mesh_GatringBullet } {

	world_ = world;

	tag_ = "EnemyBulletTag";

	name_ = "GatlingBullet";

	velocity_ = velocity;

	//弾の大きさ
	collider_ = BoundingSphere{ 0.05f };

	transform_.position(position);

	//寿命
	lifespanTimer_ = 180.0f;

	m_AttackValue = Damage;

	quatenion_.setLookRotation(velocity);
	transform_.rotation(quatenion_);
	mesh_.Transform(transform_.localToWorldMatrix());
}

void GatlingBullet::update(float delta_time) {

	mesh_.Update(delta_time);

	//寿命が尽きたら死亡
	if (lifespanTimer_ <= 0.f) {
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
		//フィールドに衝突したら死亡
		die();
		return;
	}
	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	mesh_.Transform(transform_.localToWorldMatrix());
}

void GatlingBullet::draw() const {
	mesh_.Draw();
}

void GatlingBullet::react(Actor& other) {
	if (other.tag() == "PlayerTag")die();
}