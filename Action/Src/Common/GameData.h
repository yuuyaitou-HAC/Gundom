#ifndef GAME_DATA_H_
#define GAME_DATA_H_

class GameData {
public:

	void initialize();

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

	//プレイヤーの補給状況
	bool playerSupply()const;
	void setPlayerSupply(bool frag);

	//ゲームクリアフラグ
	bool gameClearFrag()const;
	void setGameClearFrag(bool frag);

	//ミッションのクリア状態
	int missionClear()const;
	void setMissionClear(int counter);

	//銃ごとのキル数
	int beamSaberKillCounter()const;
	void setBeamSaberKillCounter(int counter);

	int beamRifleKillCounter()const;
	void setBeamRifleKillCounter(int counter);

	int beamMagnumKillCounter()const;
	void setBeamMagnumKillCounter(int counter);

	int BazookaKillCounter()const;
	void setBazookaKillCounter(int counter);

	int AllRangeUnitKillCounter()const;
	void setAllRangeUnitKillCounter(int counter);

private:

	int EnemyKillCounter_;
	bool BossMakeFrag_;
	bool BossDieFrag_;
	bool PlayerDieFrag_;
	bool BossRetreatFrag_;
	bool PlayerSupplyFrag_;
	bool GameClearFrag_;
	int MissionCounter_;
	int BeamSaberecounter_;
	int BeamRiflecounter_;
	int BeamMagnumecounter_;
	int Bazookaecounter_;
	int AllRangeUnitecounter_;
};
#endif // !GAME_DATA_H_