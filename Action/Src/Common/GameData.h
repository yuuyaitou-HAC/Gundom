#ifndef GAME_DATA_H_
#define GAME_DATA_H_

class GameData {
public:

	//GameData();

	//void update(float delta_time);

	//void draw();

	int dieEnemyCounter()const;
	void setDieEnemyCounter(int counter);

	bool bossMake()const;
	void setBossMake(bool frag);

	bool bossDie()const;
	void setBossDie(bool frag);

	bool playerDie()const;
	void setPlayerDie(bool frag);

private:

	int EnemyKillCounter_;
	bool BossMakeFrag_;
	bool BossDieFrag_;
	bool PlayerDieFrag_;

};

#endif // !GAME_DATA_H_
