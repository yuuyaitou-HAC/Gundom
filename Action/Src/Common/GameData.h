#ifndef GAME_DATA_H_
#define GAME_DATA_H_

class GameData {
public:

	//GameData();

	//void update(float delta_time);

	//void draw();

	//倒した敵部隊の数
	int dieEnemyCounter()const;
	void setDieEnemyCounter(int counter);

	//ボスの生成
	bool bossMake()const;
	void setBossMake(bool frag);

	//ボスの退却状況
	bool bossRetreat() const;
	void setBossRetreat(bool frag);

	//ボスの死亡
	bool bossDie()const;
	void setBossDie(bool frag);

	//プレイヤーの死亡
	bool playerDie()const;
	void setPlayerDie(bool frag);

	bool playerSupply()const;
	void setPlayerSupply(bool frag);

private:

	int EnemyKillCounter_;
	bool BossMakeFrag_;
	bool BossDieFrag_;
	bool PlayerDieFrag_;
	bool BossRetreatFrag_;
	bool PlayerSupplyFrag_;
};

#endif // !GAME_DATA_H_
