#include "MissileDamageRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "GSeffect.h"
#include "Common/Assets.h"

MissileDamageRange::MissileDamageRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {

	world_ = world;
	tag_ = "EnemyBulletTag";
	name_ = "MissileBullet";

	velocity_ = velocity;

	//当たり判定の生成
	collider_ = BoundingSphere{ 4.0f };

	transform_.position(position);

	m_AttackValue = Damage;

	//爆破エフェクト再生
	effect_handle_ = gsPlayEffect(Effect_ExplosionL, &position);
}

void MissileDamageRange::update(float delta_time) {

	if (!gsExistsEffect(effect_handle_)) {
		gsStopEffect(effect_handle_);
		die();
		return;
	}

	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

	gsSetEffectScale(effect_handle_, &scall);
}

void MissileDamageRange::react(Actor& other) {
	//プレイヤーと当たったときにタグを変更
	if (other.tag() == "PlayerTag") {
		tag_ = "DieTag";
	}
}
