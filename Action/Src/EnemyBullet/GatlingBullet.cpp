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
	lifespan_timer = 180.0f;

	m_AttackValue = Damage;

	quatenion.setLookRotation(velocity);
	transform_.rotation(quatenion);
	//mesh_.Transform(transform_.localToWorldMatrix());
	//effect_handle = gsPlayEffect(Effect_EnemyBullet, &position);
}

void GatlingBullet::update(float delta_time) {

	//エフェクトのサイズの調整
	//GSmatrix4 effectsize;
	//effectsize.setScale(GSvector3{ 2.0f,2.0f,2.0f });
	//エフェクトに自身のワールド変換行列を設定
	//GSmatrix4 world = effectsize * transform_.localToWorldMatrix();
	//ワールド変換行列を設定
	//gsSetEffectMatrix(effect_handle, &world);

	//mesh_.Update(delta_time);

	//寿命が尽きたら死亡
	if (lifespan_timer <= 0.f) {
		//gsStopEffect(effect_handle);
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
		//gsStopEffect(effect_handle);
		//フィールドに衝突したら死亡
		die();
		return;
	}
	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//mesh_.Transform(transform_.localToWorldMatrix());
}

void GatlingBullet::draw() const {
	//mesh_.Draw();
}

void GatlingBullet::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		//gsStopEffect(effect_handle);
		die();
	}
}