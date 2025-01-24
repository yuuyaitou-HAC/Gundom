#include "BeamMagnumBullet.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

BeamMagnumBullet::BeamMagnumBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {

	//ワールドを設定
	world_ = world;
	//タグ名
	tag_ = "PlayerBulletTag";
	//アクター名
	name_ = "BeamMagnumBullet";
	//移動量の初期化
	velocity_ = velocity;
	//衝突判定用の球体を設定
	collider_ = BoundingSphere{ 0.2f };
	//座標の初期化
	transform_.position(position);
	//寿命
	lifeSpan_time_ = 60.f;

	m_AttackValue = Damage;

}

void BeamMagnumBullet::update(float delta_time)
{
	//寿命が尽きたら死亡
	if (lifeSpan_time_ <= 0.f) {
		die();
		return;
	}
	//寿命の更新
	lifeSpan_time_ -= delta_time;

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

void BeamMagnumBullet::draw() const
{
	//デバック表示
	collider().draw();
}

void BeamMagnumBullet::react(Actor& other) {



}
