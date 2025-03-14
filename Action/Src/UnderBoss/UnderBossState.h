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
	GunState GS;

};

#endif // !BOSS_STATE_H_
