#include "BeamMagnumBullet.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "GSeffect.h"
#include "Common/Assets.h"

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
	collider_ = BoundingSphere{ 0.4f };
	//座標の初期化
	transform_.position(position);
	//寿命
	lifeSpan_time_ = 60.f;

	m_AttackValue = Damage;

	quatenion.setLookRotation(velocity);
	transform_.rotation(quatenion);

	//エフェクトを生成する
	effect_handle = gsPlayEffect(Effect_PBeamMagnum, &position);
}

void BeamMagnumBullet::update(float delta_time)
{
	//エフェクトのサイズの調整
	GSmatrix4 effectsize;
	effectsize.setScale(GSvector3{ 1.0f,1.0f,1.0f });
	//エフェクトに自身のワールド変換行列を設定
	GSmatrix4 world = effectsize * transform_.localToWorldMatrix();
	//ワールド変換行列を設定
	gsSetEffectMatrix(effect_handle, &world);

	//寿命が尽きたら死亡
	if (lifeSpan_time_ <= 0.f) {
		die();
		gsStopEffect(effect_handle);
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
		gsStopEffect(effect_handle);
		return;
	}

	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

}