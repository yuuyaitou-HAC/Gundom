#ifndef PLAYER_STATE_H_
#define PLAYER_STATE_H_

#include <gslib.h>


class PlayerState {

public:
	PlayerState() = default;

	void initialize_state_();


	//UŒ‚—Í
	int Attack()const;
	void AddAttack(int changeAT);

	//–hŒä—Í
	int Defense()const;
	void AddDefense(int changeDE);

	//HP
	int HP() const;
	void AddHP(int changeHP);

	//Å‘åHP
	int MaxHP() const;
	void AddMaxHP(int changeMaxHP);

	//ˆÚ“®‘¬“x
	float MoveSpeed()const;
	void AddMoveS(float changeMS);

	float Enargy()const;
	void AddEnargy(float changeE);

};


#endif // !PLAYER_STATE_H_
