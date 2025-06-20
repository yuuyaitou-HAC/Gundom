#ifndef BOSS_DAMAGE_RANGE_H_
#define BOSS_DAMAGE_RANGE_H_

#include "Collision/BasicAttackCollider.h"

class Boss;

class BossDamageRange : public BasicAttackCollider {

public:

	//攻撃の種類
	enum class EffectState {
		Dust,
		Slash
	};

	BossDamageRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage, BossDamageRange::EffectState effectstate, float radiuse);

	~BossDamageRange();

	virtual void update(float delta_time)override;

	virtual void react(Actor& other)override;

private:

	//エフェクトステータス
	EffectState effectState_;

	//寿命
	float lifeSpan_ = 180.0f;

	//エフェクト関係
	GSmatrix4 effectWorld_;
	GSmatrix4 effectLocalMatrix_;
	GSuint effectHandle_;

	GSmatrix4 impactWorld_;
	GSmatrix4 impactLocalMatrix_;

	//衝撃エフェクト
	GSuint impactEffect_;

	//砂埃エフェクトが終了したかどうか
	bool sandFinishFrag_;

	//衝撃エフェクトが終了したかどうか
	bool impactFinishFrag_;

	//砂埃の回転
	GSvector3 sandRotate_;

	//砂埃の大きさ
	GSvector3 sandScale_{ 4.0f,4.0f,4.0f };

	//薙ぎ払いの回転
	GSvector3 cleaverRotate_{ 0.0f,90.0f,0.0f };

	//薙ぎ払いの大きさ
	GSvector3 ceaverScale_{ 2.0f,2.0f,2.0f };

	//衝撃の回転
	GSvector3 impactRotate_;

	//衝撃の大きさ
	GSvector3 impactScale_{ 3.0f,3.0f,3.0f };

	//ボス
	Boss* boss_;

	//砂埃エフェクトの色
	GScolor4 dustEffectColor_{ 0.5f,0.42f, 0.33f, 1.0f };

	//斬撃エフェクトの再生座標
	GSvector3 slashEffectPos_{ 0,2,0 };

	//斬撃エフェクトの色
	GScolor4 slashEffectColor_{ 0.86f,0.298f,1.0f,1.0f };
};
#endif // !BOSS_DAMAGE_RANGE_H_