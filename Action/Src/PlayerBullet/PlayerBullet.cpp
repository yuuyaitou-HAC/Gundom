#include "PlayerBullet/PlayerBullet.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "GSeffect.h"
#include "Common/Assets.h"

//コンストラクタ
PlayerBullet::PlayerBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int damage, std::string name) {
	//ワールドを設定
	world_ = world;
	//タグ名
	tag_ = "PlayerBulletTag";
	//アクター名
	name_ = name;
	//移動量の初期化
	velocity_ = velocity;
	//衝突判定用の球体を設定
	collider_ = BoundingSphere{ 0.2f };
	//寿命
	lifespan_timer_ = 60.f;

	m_AttackValue = damage;

	quatenion.setLookRotation(velocity);
	transform_.rotation(quatenion);

	//座標の初期化
	transform_.position(position);
	//エフェクトを生成する
	effect_handle = gsPlayEffect(Effect_PBeamRifle, &position);
}

//更新
void PlayerBullet::update(float delta_time) {

	//エフェクトのサイズの調整
	GSmatrix4 effectsize;
	effectsize.setScale(GSvector3{ 2.0f,2.0f,2.0f });
	//エフェクトに自身のワールド変換行列を設定
	GSmatrix4 world = effectsize * transform_.localToWorldMatrix();
	//ワールド変換行列を設定
	gsSetEffectMatrix(effect_handle, &world);

	//寿命が尽きたら死亡
	if (lifespan_timer_ <= 0.f) {
		gsStopEffect(effect_handle);
		die();
		return;
	}
	//寿命の更新
	lifespan_timer_ -= delta_time;
	//フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		//交点の座標に補正
		transform_.position(intersect);
		//フィールドに衝突したら死亡
		gsStopEffect(effect_handle);
		die();
		return;
	}
	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

//描画
void PlayerBullet::draw()const {


}

void PlayerBullet::die() {

	gsStopEffect(effect_handle);

	Actor::die();
}

//衝突リアクション
void PlayerBullet::react(Actor& other) {

	if (other.tag() == "EnemyTag") {
		//エフェクトの停止
		gsStopEffect(effect_handle);
		//衝突したら死亡
		die();
	}
}