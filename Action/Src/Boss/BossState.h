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
	int HP_;

	//MaxHP
	int MaxHP_;

	//UŒ‚—Í
	int attack_;

	//–hŒä—Í
	int defense_;

	//ˆÚ“®‘¬“x
	float speed_;

	//‰Šú‰»—p
	int initializeHP_{ 1000 };
	int initializeMaxHP_{ 1000 };
	int initializeAttack_{ 50 };
	int initializeDefense_{ 25 };
	float initializeSpeed_{ 0.3f };

};
#endif // !BOSS_STATE_H_