#include "BossDamageRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "GSeffect.h"
#include "Common/Assets.h"
#include "Boss/Boss.h"

BossDamageRange::BossDamageRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage, int effectNum, float radius)
{
	world_ = world;

	tag_ = "EnemyBulletTag";
	name_ = "EnemyDamageRange";
	velocity_ = velocity;

	collider_ = BoundingSphere{ radius };

	effectNum_ = effectNum;

	transform_.position(position);
	boss_ = static_cast<Boss*>(world_->find_actor("Boss"));

	//ダメージ量
	m_AttackValue = Damage;

	if (effectNum == 1) {
		//砂埃
		effectHandle_ = gsPlayEffect(Effect_FootDustL, &position);
		effectimpact_ = gsPlayEffect(Effect_Impact, &position);
	}
	else {
		//薙ぎ払い
		effectHandle_ = gsPlayEffect(Effect_SlashGray, &position);
	}
}

//デストラクタ
BossDamageRange::~BossDamageRange() {
	gsStopEffect(effectHandle_);
	gsStopEffect(effectimpact_);
}

void BossDamageRange::update(float delta_time) {

	//寿命
	lifeSpan_ -= delta_time;

	//エフェクトが終了したら
	if (effectNum_ == 1) {

		//それぞれのエフェクトが終了したかどうか
		sandFinishFrag_ = gsExistsEffect(effectHandle_);
		impactFinishFrag_ = gsExistsEffect(effectimpact_);

		//エフェクト再生し終えたら死亡する
		if (!sandFinishFrag_ && !impactFinishFrag_) {
			die();
			return;
		}
	}
	else {
		//一定時間経ったら
		if (lifeSpan_ <= 0.0f) {
			die();
			return;
		}
	}

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

	//エフェクトの種類に応じて各種設定を変える
	if (effectNum_ == 1) {

		//砂埃エフェクト
		effectLocalMatrix_ = GSmatrix4::TRS(GSvector3::zero(), GSquaternion::euler(sandRotate_), sandScale_);
		//砂埃エフェクトを地面の色に近づける
		GScolor  effectcolor = GScolor{ 0.5f,0.42f, 0.33f, 1.0f };
		gsSetEffectColor(effectHandle_, &effectcolor);

		//衝撃エフェクト
		impactLocalMatrix_ = GSmatrix4::TRS(boss_->transform().forward() * 2, GSquaternion::euler(impactRotate_), impactScale_);
		impactWorld_ = impactLocalMatrix_ * transform_.localToWorldMatrix();
		gsSetEffectMatrix(effectimpact_, &impactWorld_);
		//再生速度を遅くする
		gsSetEffectSpeed(effectimpact_, 0.5f);
	}
	else {
		effectLocalMatrix_ = GSmatrix4::TRS(GSvector3{ 0.0f,2.0f,0.0f } - boss_->transform().forward() * 3, GSquaternion::euler(cleaverRotate_ + boss_->transform().forward()), ceaverScale_);
		//再生速度を遅くする
		gsSetEffectSpeed(effectHandle_, 0.1f);

		//色合いを残像らしくする
		GScolor  effectcolor = GScolor{ 0.86f,0.298f,1.0f,1.0f };
		gsSetEffectColor(effectHandle_, &effectcolor);

	}

	effectWorld_ = effectLocalMatrix_ * transform_.localToWorldMatrix();
	gsSetEffectMatrix(effectHandle_, &effectWorld_);
}

void BossDamageRange::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		tag_ = "DieTag";
	}
}
