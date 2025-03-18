#ifndef GAME_DATA_H_
#define GAME_DATA_H_

class GameData {
public:

	void initialize();

	//倒した敵部隊の数
	int dieEnemyCounter()const;
	void setDieEnemyCounter(int counter);

	//中ボスの生成
	bool underBossMake()const;
	void setUnderBossMake(bool frag);

	//ボスの生成
	bool bossMake()const;
	void setBossMake(bool frag);

	//ボスの退却状況
	bool bossRetreat() const;
	void setBossRetreat(bool frag);

	//中ボスの死亡
	bool underBossDie()const;
	void setUnderBossDie(bool frag);

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

	int bazookaKillCounter()const;
	void setBazookaKillCounter(int counter);

	int allRangeUnitKillCounter()const;
	void setAllRangeUnitKillCounter(int counter);

	//撤退
	bool withdrawalInstructions()const;
	void setWithdrawalInstructions(bool frag);

	//配列まで調整終了
	bool incomplete() const;
	void setIncomplete(bool frag);


private:

	//敵のキル数
	int enemyKillCounter_;
	//中ボスの生成
	bool underBossMakeFrag_;
	//ボスの生成	
	bool bossMakeFrag_;
	//中ボスの死亡	
	bool underBossDieFrag_;
	//ボスの死亡	
	bool bossDieFrag_;
	//プレイヤーの死亡	
	bool playerDieFrag_;
	//中ボスの退却	
	bool underBossRetreatFrag_;
	//プレイヤーの補給	
	bool playerSupplyFrag_;
	//ゲームクリア	
	bool gameClearFrag_;

	bool retreatFrag_;
	//ミッションのカウント	
	int missionCounter_;
	//ビームサーベルのキル数	
	int beamSaberecounter_;
	//ビームライフルのキル数
	int beamRiflecounter_;
	//ビームマグナムのキル数	
	int beamMagnumecounter_;
	//バズーカのキル数	
	int bazookaecounter_;
	//ファンネルのキル数	
	int allRangeUnitecounter_;
};
#endif // !GAME_DATA_H_