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
	void addExSkillPoint(int point);

	//必殺発動時のステータス上昇
	void setEXSkill(float magnification);

	//必殺終了時に元のステータスに戻す
	void resetEXSkill();

private:
	GunState gs_;

	//攻撃力
	int attackValue_{ 30 };
	const int attackValueInitialize_{ 30 };

	//防御力
	int defenseValue_{ 20 };
	const int defenseValueInitialize_{ 20 };

	//HP
	int hpValue_{ 100 };
	const int hpValueInitialize_{ 100 };

	//最大HP	
	int maxHPValue_{ 100 };
	const int maxHPValueInitialize_{ 100 };

	//元のスピード 0.15
	float walkSpeed_{ 0.15f };
	const float walkSpeedInitialize_{ 0.15f };

	//エネルギー量
	float energyValue_{ 100.0f };
	const float energyValueInitialize_{ 100.0f };

	float maxEnergyValue_{ 100.0f };
	const float maxEnergyValueInitialize_{ 100.0f };

	float exMag_{ 0.0f };

	//ビームライフルの弾
	int beamBullet_{ 20 };
	const int beamBulletInitialize_{ 20 };

	//ビームマグナムの弾
	int beamMagnamBullet_{ 7 };
	const int beamMagnamBulletInitialize_{ 7 };

	//ビームマグナムのマガジン数
	int beamMagnamMagazin_{ 3 };
	const int beamMagnamMagazinInitialize_{ 3 };

	//バズーカの弾
	int bazookaBullet_{ 3 };
	const int bazookaBulletInitialize_{ 3 };

	//バズーカのマガジン数
	int bazookaMagazin_{ 3 };
	const int bazookaMagazinInitialize_{ 3 };

	//EXスキルポイント
	int exSkillPoint_{ 0 };

	int nowAttackValue_{ 0 };
	int nowDefenceValue_{ 0 };
	float nowSpeed_{ 0 };
	float nowEnargy_{ 0 };
};
#endif // !PLAYER_STATE_H_