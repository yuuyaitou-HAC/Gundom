#ifndef BOSS_STATE_H_
#define BOSS_STATE_H_

#include <gslib.h>

class BossState {

public:
	//弾の種類
	enum class GunState {
		Beamlifl,	//通常弾
		Basterlifl,	//バスターライフル
		Gatling,//ガトリング
	};

public:

	BossState() = default;

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
	GunState gunstate_();
	void SetGunState(GunState gunstate);

	//ビームライフルの残弾
	int BeamBullet();
	void SetBeamBullet(int Bullet);

	//ガトリングの残弾
	float GatlingBullet();
	void SetGatlingBullet(float Bullet);

	//バスターライフルの残弾
	int BasterBullet();
	void SetBasterBullet(int Bullet);

private:
	GunState GS;

};

#endif // !BOSS_STATE_H_
