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
};

#endif // !BOSS_STATE_H_