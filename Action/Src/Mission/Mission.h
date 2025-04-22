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

	void playerstateup(float magnification);

private:

	Player* player_;

	UnderBoss* underBoss_;

	Boss* boss_;

	State state_;

private:

	//ミッションに使うタイマー
	float MissionTimer;

	//次のミッションへの移行を遅らせるタイマー
	float delay_timer = 300.0f;

	//移行タイマーの代入用
	float Assignmentdelay_timer = 300.0f;

	//ミッション前のキル数
	int beforKillCounter_;

	//ミッション中のキル数
	int MissionKillCounter_;

	//画像の色指定
	GScolor4 TextureColor{ 256,256,256,1.0f };

	GSvector2 missionBackPosition_{ 80,50 };
	GSrect missionBackRect_{ 0,0,800,600 };//400 240  tyusinn  200 120
	GSvector2 missionBackScale_{ 0.5,0.4 };

	//HPバー(青)
	GSvector2 HPposition{ 700,100 };
	GSrect HPRect{ 0,0,500,20 };
	GSvector2 HPScale{ 1,1 };

	//HPバー(灰)
	GSvector2 HPBackposition{ 1200,120 };
	GSrect HPBackRect{ 0,0,500,20 };
	float HPBarScale;

	//ミッション内容
	GSvector2 missionPosition_{ 120,70 };
	GSrect missionRect_{ 0,0,320,200 }; // 160 100
	GSvector2 missionScale_{ 1.0,1.0 };

	//撃破数の表示
	GSvector2 killnumPosition_{ 700,100 };
	GSrect killnumRect_{ 0,0,300,100 };
	GSvector2 killnumScale_{ 0.5,0.5 };

	//スラッシュ
	GSvector2 slashPosition_{ 950,100 };
	GSrect slashRect_{ 0,0,60,60 };
	GSvector2 slashScale_{ 0.9,0.9 };

	//数値
	mutable GSvector2 numpos;
	GSvector2 numScale_{ 0.9,0.9 };
	GSrect NumRect[10]{
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

};
#endif // !MISSION_H_