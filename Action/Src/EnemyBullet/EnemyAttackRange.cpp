#include "EnemyAttackRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "GSeffect.h"

EnemyAttackRange::EnemyAttackRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {

	world_ = world;

	tag_ = "EnemyBulletTag";

	name_ = "EnemyAttackRange";

	velocity_ = velocity;

	collider_ = BoundingSphere{ 2.0f };

	transform_.position(position);
	//攻撃力
	m_AttackValue = Damage;
	//エフェクト
	effectHandle_ = gsPlayEffect(Effect_SlashGray, &position);
}

void EnemyAttackRange::update(float delta_time) {

	//エフェクトの再生が終了したか？
	if (gsExistsEffect(effectHandle_)) {
		die();
	}
	//エフェクトの色適応
	gsSetEffectColor(effectHandle_, &slashEffectColor_);
}

//当たり判定
void EnemyAttackRange::react(Actor& other) {
	//プレイヤーに当たったらタグ変更
	if (other.tag() == "PlayerTag") {
		tag_ = "DieTag";
	}
}