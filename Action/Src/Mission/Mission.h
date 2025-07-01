#ifndef MISSION_H_
#define MISSION_H_

#include "Actor/Actor.h"
#include "Player/Player.h"
#include "UnderBoss/UnderBoss.h"
#include "Boss/Boss.h"
#include "Common/GameData.h"

class Mission : public Actor {

	enum class State {
		Mission1,
		Mission2,
		Mission3,
		Mission4,
		GameClear
	};

public:

	Mission(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time) override;

	virtual void draw_gui()const override;

private:

	void mission1(float delta_time);

	void mission2(float delta_time);

	void mission3(float delta_time);

	void mission4(float delta_time);

	void gameClear(float delta_time);

	void playerstateup(float magnification, float DefenceMagnification);

private:

	Player* player_{ NULL };

	UnderBoss* underBoss_{ NULL };

	Boss* boss_{ NULL };

	State state_{State::Mission1 };

private:

	//ミッション１のノルマ
	const int enemyDieCounter_{ 5 };

	//ミッション前のキル数
	int beforKillCounter_{ 0 };

	//ミッション中のキル数
	int missionKillCounter_{ 0 };

	//ミッションに使うタイマー
	float mission3Timer_{ 7200.0f };//7200

	//次のミッションへの移行を遅らせるタイマー
	float delayTimer_{ 420.0f };

	//バフ付与条件
	const int missionBuffKILL_[5]{ 5,10,15,20,25};

	//バフ付与の倍率
	const float missionBuffMagnification_[6]{ 1.2f,1.5f,1.8f,2.1f,2.4f,2.7f };
	const float missionBuffMagnificationDefance_[6]{ 1.05f,1.1f,1.2f,1.3f,1.4f,1.5f };

	//移行タイマーの代入用
	const float assignmentdelayTimer_{ 420.0f };

	//画像の色指定
	const GScolor4 textureColor_{ 256,256,256,1.0f };

	//ミッション表示背景
	const GSvector2 missionBackPosition_{ 80,50 };
	const GSrect missionBackRect_{ 0,0,800,600 };//400 240  tyusinn  200 120
	const GSvector2 missionBackScale_{ 0.5,0.4 };

	//HPバー(青)
	const GSvector2 hpposition_{ 740,100 };
	const GSrect hpRect_{ 0,0,500,20 };
	const GSvector2 hpScale_{ 1,2.5 };

	//HPバー(灰)
	const GSvector2 hpBackposition_{ 1240,150 };
	const GSrect hpBackRect_{ 0,0,500,20 };
	float hpBarScale_{0};

	//ミッション内容
	const GSvector2 missionPosition_{ 120,70 };
	const GSrect missionRect_{ 0,0,320,200 };
	const GSvector2 missionScale_{ 1.0,1.0 };

	//撃破数の表示
	const GSvector2 killnumPosition_{ 750,150 };
	const GSrect killnumRect_{ 0,0,300,100 };
	const GSvector2 killnumScale_{ 0.5,0.5 };

	//スラッシュ
	const GSvector2 slashPosition_{ 1000,150 };
	const GSrect slashRect_{ 0,0,60,60 };
	const GSvector2 slashScale_{ 0.9,0.9 };


	const GSvector2 numScale_{ 0.9,0.9 };
	const GSrect numRect_[10]{
	GSrect{0,  0, 60,  60},
	GSrect{0, 60, 60, 120},
	GSrect{0,120, 60, 180},
	GSrect{0,180, 60, 240},
	GSrect{0,240, 60, 300},
	GSrect{0,300, 60, 360},
	GSrect{0,360, 60, 420},
	GSrect{0,420, 60, 480},
	GSrect{0,480, 60, 540},
	GSrect{0,540, 60, 600}
	};

	//現在の撃破数表示座標
	const GSvector2 nowKillCountTen_{ 900,140 };
	const GSvector2 nowKillCountOne_{ 950,140 };

	//ミッション３中の撃破数
	const GSvector2 mission3KillCountTen_{ 1110,190 };
	const GSvector2 mission3KillCountOne_{ 1150,190 };

	//目標の撃破数表示座標
	const GSvector2 objectiveKillCountTen_{ 1050,140 };
	const GSvector2 objectiveKillCountOne_{ 1100,140 };

	//現在の中ボスHPの表示座標
	const GSvector2 nowUndreBossHPHundred_{ 905,100 };
	const GSvector2 nowUndreBossHPTen_{ 930,100 };
	const GSvector2 nowUndreBossHPOne_{ 955,100 };

	//MAX時の中ボスHPの表示座標
	const GSvector2 maxUndreBossHPHundred_{ 1005,100 };
	const GSvector2 maxUndreBossHPTen_{ 1030,100 };
	const GSvector2 maxUndreBossHPOne_{ 1055,100 };

	//現在のボスHPの表示座標
	const GSvector2 nowBossHPthousand_{ 880,100 };
	const GSvector2 nowBossHPHundred_{ 905,100 };
	const GSvector2 nowBossHPTen_{ 930,100 };
	const GSvector2 nowBossHPOne_{ 955,100 };

	//MAX時のボスHPの表示座標
	const GSvector2 maxBossHPthousand_{ 1005,100 };
	const GSvector2 maxBossHPHundred_{ 1030,100 };
	const GSvector2 maxBossHPTen_{ 1055,100 };
	const GSvector2 maxBossHPOne_{ 1080,100 };


	//ミッション時間の表示座標
	const GSvector2 nowMissionTimerthousand_{ 1120,120 };
	const GSvector2 nowMissionTimerhundred_{ 1160,120 };
	const GSvector2 nowMissionTimerClon_{ 1200,120 };
	const GSvector2 nowMissionTimerten_{ 1240,120 };
	const GSvector2 nowMissionTimerone_{ 1280,120 };

	//スラッシュ HP表記に使う
	const GSvector2 slashPositionHP_{ 980,100 };
	const GSrect slashRectHP_{ 0,0,60,60 };
	const GSvector2 slashScaleHP_{ 0.4,0.4 };

	//数値HP表記に使う
	const GSvector2 numScaleHP_{ 0.4,0.4 };

	//中ボスHP説明
	const GSvector2 ubHPPosition_{ 740,150 };
	const GSrect ubHPRect_{ 0,0,500,50 };
	const GSvector2 ubHPScale_{ 1,1 };

	//中ボス死亡ログ
	const GSvector2 ubKillPosition_{ 690,150 };
	const GSrect ubKillRect_{ 0,0,600,50 };
	const GSvector2 ubKillScale_{ 1,1 };

	//桁ごとの計算
	mutable GSrect thousand{ 0 };
	mutable GSrect hundreds{ 0 };
	mutable GSrect tens{ 0 };
	mutable GSrect ones{ 0 };

	//エンターキー
	const GSrect enterRect_{ 0,0,800,200 };
	const GSvector2 enterScal_{ 1.0f,1.0f };
	const GSvector2 enterPos_{ 600,100 };

	//ミッション３の間に壊滅させた敵の数
	const GSvector2 killnum2Position_{ 600,200 };
	const GSrect killnum2Rect_{ 0,0,600,50 };
	const GSvector2 killnum2Scale_{ 1.0,1.0 };

	//ミッションの時間
	const GSvector2 mtPosition_{ 600,130 };
	const GSrect mtRect_{ 0,0,600,50 };
	const GSvector2 mtScale_{ 1.0,1.0 };

	//ミッション３用の数値の大きさ
	const GSvector2 mission3NumScale_{ 0.83,0.83 };
};
#endif // !MISSION_H_