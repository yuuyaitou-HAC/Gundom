#include "BossDamageRange.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "GSeffect.h"
#include "Common/Assets.h"
#include "Boss/Boss.h"

BossDamageRange::BossDamageRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage, int effectNum)
{
	world_ = world;

	tag_ = "EnemyBulletTag";
	name_ = "BossBulet";
	velocity_ = velocity;

	collider_ = BoundingSphere{ 4.0 };

	effectNum_ = effectNum;

	transform_.position(position);
	boss_ = static_cast<Boss*>(world_->find_actor("Boss"));


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

void BossDamageRange::update(float delta_time) {

	//エフェクトが終了したら

	if (effectNum_ == 1) {

		//それぞれのエフェクトが終了したかどうか
		sandFinishFrag_ = gsExistsEffect(effectHandle_);
		impactFinishFrag_ = gsExistsEffect(effectimpact_);

		if (!sandFinishFrag_ && !impactFinishFrag_) {
			gsStopEffect(effectHandle_);
			gsStopEffect(effectimpact_);
			die();
			return;
		}
	}
	else {
		if (!gsExistsEffect(effectHandle_)) {
			gsStopEffect(effectHandle_);
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
		effectLocalMatrix_ = GSmatrix4::TRS(GSvector3::zero(), GSquaternion::euler(sandRotate_), sandScale_);
		GScolor  effectcolor = GScolor{ 0.5f,0.42f, 0.33f, 1.0f };
		gsSetEffectColor(effectHandle_, &effectcolor);

		impactLocalMatrix_ = GSmatrix4::TRS(boss_->transform().forward() * 2, GSquaternion::euler(impactRotate_), impactScale_);
		impactWorld_ = impactLocalMatrix_ * transform_.localToWorldMatrix();
		gsSetEffectMatrix(effectimpact_, &impactWorld_);
		gsSetEffectSpeed(effectimpact_, 0.5f);
	}
	else {
		effectLocalMatrix_ = GSmatrix4::TRS(GSvector3::zero(), GSquaternion::euler(cleaverRotate_), ceaverScale_);
	}

	effectWorld_ = effectLocalMatrix_ * transform_.localToWorldMatrix();
	gsSetEffectMatrix(effectHandle_, &effectWorld_);

}

void BossDamageRange::draw() const {
	collider().draw();
}

void BossDamageRange::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		tag_ = "DieTag";
	}
}
