#ifndef UNDER_BOSS_STATE_H_
#define UNDER_BOSS_STATE_H_

#include <gslib.h>

class UnderBossState {

public:
	//弾の種類
	enum class GunState {
		Beamlifl,	//通常弾
		Basterlifl,	//バスターライフル
		Gatling,//ガトリング
	};

public:

	UnderBossState() = default;

	void initialize_state_();

	//攻撃力
	int Attack()const;
	void AddAttack(int changeAT);

	//防御力
	int Defense()const;
	void AddDefense(int changeDE);

	//HP
	int HP() const;
	void AddHP(int changeHP);

	//最大HP
	int MaxHP() const;
	void AddMaxHP(int changeMaxHP);

	//移動速度
	float MoveSpeed()const;
	void AddMoveS(float changeMS);

	//エネルギー
	float Enargy()const;
	void AddEnargy(float changeE);

	//銃の種類
	GunState gunstate_()const;
	void SetGunState(GunState gunstate);

	//ビームライフルの残弾
	int BeamBullet()const;
	void SetBeamBullet(int Bullet);

	//ガトリングの残弾
	int GatlingBullet()const;
	void SetGatlingBullet(int Bullet);

	//バスターライフルの残弾
	int BasterBullet()const;
	void SetBasterBullet(int Bullet);

	bool BasterFrag()const;
	void SetBasterFrag(bool frag);

private:
	GunState GS{ GunState::Beamlifl };

	//攻撃力
	int underBossAttackValue_{ 40 };
	const int assignmentUnderBossAttackValue_{ 40 };

	//防御力
	int underBossDefenseValue_{ 18 };
	const int assignmentUnderBossDefenseValue_{ 18 };

	//HP
	int underBossHPValue_{ 200 };
	const int assignmentUnderBossHPValue_{ 200 };

	//最大HP	
	int underBossMaxHPValue_{ 200 };
	const int assignmentUnderBossMaxHPValue_{ 200 };

	//元のスピード
	float underBossWalkSpeed_{ 0.15f };
	const float assignmentUnderBossWalkSpeed_{ 0.15f };

	//エネルギー量
	float underBossEnergyValue_{ 100.0f };
	const float assignmentUnderBossEnergyValue_{ 100.0f };

	//ビームライフルの弾
	int underBossBB_{ 20 };
	const int assignmentUnderBossBB_{ 20 };

	//ガトリングの弾
	int GB_{ 200 };
	const int assignmentGB_{ 200 };

	//バスターライフルの弾
	int BLB_{ 1 };
	const int assignmentBLB_{ 1 };

	bool BasterFrag_{ false };
};
#endif // !BOSS_STATE_H_