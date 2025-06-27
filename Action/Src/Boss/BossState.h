#ifndef BOSS_STATE_H_
#define BOSS_STATE_H_

class BossState {

public:

	BossState() = default;

public:

	//‰Šú‰»
	void initialize_state_();

	//HP
	int HP()const;
	void AddHP(int value);

	//Å‘åHP
	int MaxHP()const;
	void AddMaxHp(int value);

	//–hŒä—Í
	int defense()const;
	void addDefense(int value);

	//UŒ‚
	int attack()const;
	void addAttack(int value);

	//ˆÚ“®‘¬“x
	float moveSpeed()const;
	void addMoveS(float value);

private:
	//HP
	int HP_{ 1000 };

	//MaxHP
	int MaxHP_{ 1000 };

	//UŒ‚—Í
	int attack_{ 50 };

	//–hŒä—Í
	int defense_{ 35 };

	//ˆÚ“®‘¬“x
	float speed_{ 0.3f };

	//‰Šú‰»—p
	const int initializeHP_{ 1000 };
	const int initializeMaxHP_{ 1000 };
	const int initializeAttack_{ 50 };
	const int initializeDefense_{ 35 };
	const float initializeSpeed_{ 0.3f };

};
#endif // !BOSS_STATE_H_