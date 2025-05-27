#include "DamageRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "GSeffect.h"
#include "Common/Assets.h"

DamageRange::DamageRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage)
{
	//ワールドを設定
	world_ = world;
	//タグ名
	tag_ = "PlayerBulletTag";
	//アクター名
	name_ = "BazookaBullet";
	//移動量の初期化
	velocity_ = velocity;
	//衝突判定用の球体を設定
	collider_ = BoundingSphere{ 4.0f };
	//座標の初期化
	transform_.position(position);

	m_AttackValue = Damage;

	//爆破エフェクト再生
	effect_handle = gsPlayEffect(Effect_ExplosionL, &position);
	gsSetVolumeSE(SE_BazoocaExplosion, 0.8f);
	gsPlaySE(SE_BazoocaExplosion);
}

void DamageRange::update(float delta_time)
{
	if (!gsExistsEffect(effect_handle)) {
		gsStopEffect(effect_handle);
		die();
		return;
	}

	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

	gsSetEffectScale(effect_handle, &scall);
}

void DamageRange::react(Actor& other)
{
	if (other.tag() == "EnemyTag") {
		//gsStopSE(SE_BazoocaExplosion);
		tag_ = "DieTag";
	}
}