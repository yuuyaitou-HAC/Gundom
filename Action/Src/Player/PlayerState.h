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

	float MaxEnatgy()const;
	void addMaxEnargy(float maxenargy);

	//銃の種類
	GunState gunstate_();
	void setGunState(GunState gunstate);

	//ビームライフルの残弾
	int beamBullet();
	void setBeamBullet(int Bullet);

	//ビームマグナムの残弾
	int beamMagnumBullet();
	void setBeamMagnumBullet(int Bullet);

	//バズーカの残弾
	int bazookaBullet();
	void setBazookaBullet(int Bullet);

	//ビームマグナムのマガジン
	int beamMagnamMagazin();
	void setBeamMagnamMagazin(int Magazin);

	//バズーカのマガジン
	int bazookaMagazin();
	void setBazookaMagazin(int Magazin);

	//必殺技ポイント
	int exSkillPoint();
	void setExSkillPoint(int point);

	//必殺発動時のステータス上昇
	void setEXSkill(float magnification);

	//必殺終了時に元のステータスに戻す
	void resetEXSkill();

private:
	GunState GS;

	int nowAttackValue_;
	int nowDefenceValue_;
	float nowSpeed_;
	float nowEnargy_;

};


#endif // !PLAYER_STATE_H_
