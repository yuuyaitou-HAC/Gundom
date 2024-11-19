#ifndef PLAYER_STATE_H_
#define PLAYER_STATE_H_

#include <gslib.h>


class PlayerState {

public:

	//弾の種類
	enum class GunState {
		Beamlifl,	//通常弾
		BeamMagnumBullet,	//ビームマグナム
		BazookaBullet,	//バズーカ
	};

public:
	PlayerState() = default;

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

	//ビームマグナムの残弾
	int BeamMagnumBullet();
	void SetBeamMagnumBullet(int Bullet);

	//バズーカの残弾
	int BazookaBullet();
	void SetBazookaBullet(int Bullet);

	int BeamMagnamMagazin();
	void SetBeamMagnamMagazin(int Magazin);

	int BazookaMagazin();
	void SetBazookaMagazin(int Magazin);


private:
	GunState GS;

};


#endif // !PLAYER_STATE_H_
