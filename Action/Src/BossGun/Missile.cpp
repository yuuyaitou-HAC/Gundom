#include "Missile.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common\Assets.h"
#include "GSeffect.h"
#include "GSmath.h"
#include "Collision/Ray.h"
#include "MissileDamageRange.h"

Missile::Missile(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) :
	mesh_{ Mesh_MissileBullet,Mesh_MissileBullet ,Mesh_MissileBullet } {

	world_ = world;

	//タグ
	tag_ = "EnemyBulletTag";
	//名前
	name_ = "MissileBullet";

	velocity_ = velocity;

	//当たり判定
	collider_ = BoundingSphere{ 0.2f };

	//回転角度の調整
	GSquaternion a;
	a.setLookRotation(velocity);
	transform_.rotation(a);

	//座標の初期化
	transform_.position(position);

	//ダメージ量
	m_AttackValue = Damage;

	mesh_.Transform(transform_.localToWorldMatrix());

	//プレイヤー取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//着弾目標地点
	targetPoint_ = player_->transform().position() + GSvector3{ (float)gsRand(-randpos_,randpos_),0.0f,(float)gsRand(-randpos_,randpos_) };

	//着弾目標地点のy軸の設定
	Ray ray = { targetPoint_,-(transform_.up()) };
	GSvector3 intersect;
	world_->field()->collide(ray, targetPoint_.y + 30.0f, &intersect);
	targetPoint_.y = intersect.y;

	//ボス取得
	boss_ = static_cast<Boss*>(world_->find_actor("Boss"));

	//ボスのy軸取得
	bossY_ = boss_->transform().position().y;

	//落下予想地点のエフェクト
	targetPointEffect_ = gsPlayEffect(Effect_DropPoint, &targetPoint_);

	//着弾目標地点のエフェクトを赤色にする
	gsSetEffectColor(targetPointEffect_, &color_);
	gsSetEffectScale(targetPointEffect_,&scall_);

	//バーニアエフェクト
	vernierEffect_ = gsPlayEffect(Effect_Ballistic, &position);
}

Missile::~Missile() {
	//エフェクト停止
	gsStopEffect(targetPointEffect_);
	gsStopEffect(vernierEffect_);
}

void Missile::update(float delta_time) {

	//回転角度の調整
	GSquaternion a;
	a.setLookRotation(velocity_);
	transform_.rotation(a);
		
	//エフェクトに自身のワールド変換行列を設定
	effectWorld_ = transform_.localToWorldMatrix();
	//ワールド変換行列を設定
	gsSetEffectMatrix(vernierEffect_, &effectWorld_);

	//フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		//爆風当たり判定生成
		if (!explosion_) {
			world_->add_actor(new MissileDamageRange{ world_,transform_.position(),GSvector3().zero(),boss_->bossState_()->attack() * 4 });
			explosion_ = true;
		}

		//交点の座標に補正
		transform_.position(intersect);
		gsStopEffect(targetPointEffect_);
		gsStopEffect(vernierEffect_);
		//フィールドに衝突したら死亡
		die();
		return;
	}

	float test = transform_.position().y - bossY_;

	//十分な高さに到達したら目標地点に向かって移動する
	if (test >= 30 && !upFrag_) {
		nowTargetPoint_ = targetPoint_ - transform_.position();
		velocity_ = nowTargetPoint_.normalized();
		upFrag_ = true;
	}

	transform_.translate(velocity_ * 1.5f * delta_time, GStransform::Space::World);

	mesh_.Transform(transform_.localToWorldMatrix());
}

void Missile::draw() const {
	mesh_.Draw();
}

void Missile::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		//爆風当たり判定生成
		if (!explosion_) {
			world_->add_actor(new MissileDamageRange{ world_,transform_.position(),GSvector3().zero(),boss_->bossState_()->attack() * 4 });
			explosion_ = true;
		}
		gsStopEffect(targetPointEffect_);
		gsStopEffect(vernierEffect_);		//衝突したら死亡
		die();
	}
}
