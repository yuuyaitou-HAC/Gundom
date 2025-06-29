#include "AttackRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

AttackRange::AttackRange(IWorld* world, const GSvector3& positon, const GSvector3& velocity, int Damage)
{
	//ワールドを設定
	world_ = world;
	//タグ名
	tag_ = "PlayerBulletTag";
	//アクター名
	name_ = "BeamSaberBullet";
	//移動量の初期化
	velocity_ = velocity;
	//衝突判定用の球体を設定
	collider_ = BoundingSphere{ 1.3f };
	//座標の初期化
	transform_.position(positon);

	m_AttackValue = Damage;
}

void AttackRange::update(float delta_time) {
	//寿命が尽きたら死亡
	if (lifeSpanTime_ <= 0.f) {
		die();
		return;
	}
	//寿命の更新
	lifeSpanTime_ -= delta_time;
}
void AttackRange::react(Actor& other) {

	if (other.tag() == "EnemyTag")die();
}
