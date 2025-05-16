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

	//初期化
	void initialize_state_();

	//補給
	void supply();

	//攻撃力
	int attack()const;
	void addAttack(int changeAT);

	//防御力
	int defense()const;
	void addDefense(int changeDE);

	//HP
	int hp() const;
	void AddHP(int changeHP);

	//最大HP
	int maxHP() const;
	void addMaxHP(int changeMaxHP);

	//移動速度
	float moveSpeed()const;
	void addMoveS(float changeMS);

	//エネルギー
	float enargy()const;
	void addEnargy(float changeE);

	float MaxEnargy()const;
	void addMaxEnargy(float maxenargy);

	//銃の種類
	GunState gunstate_()const;
	void setGunState(GunState gunstate);

	//ビームライフルの残弾
	int beamBullet()const;
	void setBeamBullet(int Bullet);

	//ビームマグナムの残弾
	int beamMagnumBullet()const;
	void setBeamMagnumBullet(int Bullet);

	//バズーカの残弾
	int bazookaBullet()const;
	void setBazookaBullet(int Bullet);

	//ビームマグナムのマガジン
	int beamMagnamMagazin()const;
	void setBeamMagnamMagazin(int Magazin);

	//バズーカのマガジン
	int bazookaMagazin()const;
	void setBazookaMagazin(int Magazin);

	//必殺技ポイント
	int exSkillPoint()const;
	void setExSkillPoint(int point);

	//必殺発動時のステータス上昇
	void setEXSkill(float magnification);

	//必殺終了時に元のステータスに戻す
	void resetEXSkill();

private:
	GunState GS;

	//攻撃力
	int AttackValue_ = 25;

	//防御力
	int DefenseValue_ = 20;

	//HP
	int HPValue_ = 100;

	//最大HP	
	int MaxHPValue_ = 100;

	//元のスピード 0.15
	float WalkSpeed_ = 0.15f;

	//エネルギー量
	float EnergyValue_ = 100.0f;

	float MaxEnergyValue_ = 100.0f;

	float exMag_;

	//ビームライフルの弾
	int BeamBullet_ = 20;

	//ビームマグナムの弾
	int BeamMagnamBullet_ = 7;

	//ビームマグナムのマガジン数
	int BeamMagnamMagazin_ = 3;

	//バズーカの弾
	int BazookaBullet_ = 3;

	//バズーカのマガジン数
	int BazookaMagazin_ = 3;

	//EXスキルポイント
	int ExSkillPoint_ = 0;

	int nowAttackValue_;
	int nowDefenceValue_;
	float nowSpeed_;
	float nowEnargy_;

};


#endif // !PLAYER_STATE_H_
