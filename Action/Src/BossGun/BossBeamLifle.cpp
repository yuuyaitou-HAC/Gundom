#include "BossBeamLifle.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "GSeffect.h"
#include "Common/Assets.h"

BossBeamLifle::BossBeamLifle(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) {
	world_ = world;

	tag_ = "EnemyBulletTag";
	name_ = "BossBullet";

	velocity_ = velocity;

	//当たり判定の大きさ
	collider_ = BoundingSphere{ 0.4f };

	lifeSpanTimer_ = 120.0f;

	m_AttackValue = Damage;

	quatenion_.setLookRotation(velocity);
	transform_.rotate(quatenion_);

	transform_.position(position);

	//エフェクト
	effectHandle_ = gsPlayEffect(Effect_DarckArrow, &position);
}

void BossBeamLifle::update(float delta_time) {

	//エフェクトのサイズの調整
	GSmatrix4 effectsize;
	effectsize.setScale(GSvector3{ 2.0f,2.0f,2.0f });
	//エフェクトに自身のワールド変換行列を設定
	GSmatrix4 world = effectsize * transform_.localToWorldMatrix();
	//ワールド変換行列を設定
	gsSetEffectMatrix(effectHandle_, &world);

	//寿命が尽きたら死亡
	if (lifeSpanTimer_ <= 0.f) {
		gsStopEffect(effectHandle_);
		die();
		return;
	}
	//寿命の更新
	lifeSpanTimer_ -= delta_time;
	//フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		//交点の座標に補正
		transform_.position(intersect);
		//フィールドに衝突したら死亡
		gsStopEffect(effectHandle_);
		die();
		return;
	}
	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

}

void BossBeamLifle::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		//エフェクトの停止
		gsStopEffect(effectHandle_);
		//衝突したら死亡
		die();
	}
}