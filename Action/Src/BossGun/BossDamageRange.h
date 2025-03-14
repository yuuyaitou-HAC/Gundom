#ifndef BOSS_DAMAGE_RANGE_H_
#define BOSS_DAMAGE_RANGE_H_

#include "Collision/BasicAttackCollider.h"

class Boss;

class BossDamageRange : public BasicAttackCollider {

public:

	BossDamageRange(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage, int effectNum);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

private:

	//エフェクトの判別
	int effectNum_;


	GSmatrix4 effectWorld_;
	GSmatrix4 effectLocalMatrix_;
	GSuint effectHandle_;

	GSmatrix4 impactWorld_;
	GSmatrix4 impactLocalMatrix_;

	//衝撃エフェクト
	GSuint effectimpact_;

	//砂埃エフェクトが終了したかどうか
	bool sandFinishFrag_;

	//衝撃エフェクトが終了したかどうか
	bool impactFinishFrag_;


	//砂埃の回転
	GSvector3 sandRotate_;

	//砂埃の大きさ
	GSvector3 sandScale_{ 4.0f,4.0f,4.0f };

	//薙ぎ払いの回転
	GSvector3 cleaverRotate_;

	//薙ぎ払いの大きさ
	GSvector3 ceaverScale_{ 2.0f,2.0f,2.0f };

	//衝撃の回転
	GSvector3 impactRotate_;

	//衝撃の大きさ
	GSvector3 impactScale_{ 3.0f,3.0f,3.0f };

	Boss* boss_;

};

#endif // !BOSS_DAMAGE_RANGE_H_
