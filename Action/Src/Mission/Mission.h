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
	float missionTimer_;

	//次のミッションへの移行を遅らせるタイマー
	float delayTimer_ = 420.0f;

	//移行タイマーの代入用
	float assignmentdelayTimer_ = 420.0f;

	//ミッション前のキル数
	int beforKillCounter_;

	//ミッション中のキル数
	int mssionKillCounter_;

	//画像の色指定
	GScolor4 textureColor_{ 256,256,256,1.0f };

	//ミッション表示背景
	GSvector2 missionBackPosition_{ 80,50 };
	GSrect missionBackRect_{ 0,0,800,600 };//400 240  tyusinn  200 120
	GSvector2 missionBackScale_{ 0.5,0.4 };

	//HPバー(青)
	GSvector2 hpposition_{ 740,100 };  
	GSrect hpRect_{ 0,0,500,20 };
	GSvector2 hpScale_{ 1,2.5 };

	//HPバー(灰)
	GSvector2 hpBackposition_{ 1240,150 };
	GSrect hpBackRect_{ 0,0,500,20 };
	float hpBarScale_;

	//ミッション内容
	GSvector2 missionPosition_{ 120,70 };
	GSrect missionRect_{ 0,0,320,200 };
	GSvector2 missionScale_{ 1.0,1.0 };

	//撃破数の表示
	GSvector2 killnumPosition_{ 750,150 };
	GSrect killnumRect_{ 0,0,300,100 };
	GSvector2 killnumScale_{ 0.5,0.5 };

	//スラッシュ
	GSvector2 slashPosition_{ 1000,150 };  
	GSrect slashRect_{ 0,0,60,60 };
	GSvector2 slashScale_{ 0.9,0.9 };

	//数値
	mutable GSvector2 numPos_;
	GSvector2 numScale_{ 0.9,0.9 };
	GSrect numRect_[10]{
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

	//スラッシュ HP表記に使う
	GSvector2 slashPositionHP_{ 980,110 };
	GSrect slashRectHP_{ 0,0,60,60 };
	GSvector2 slashScaleHP_{ 0.4,0.4 };

	//数値HP表記に使う
	mutable GSvector2 numPosHP_;
	GSvector2 numScaleHP_{ 0.4,0.4 };

	//中ボスHP説明
	GSvector2 ubHPPosition_{ 740,150 }; 
	GSrect ubHPRect_{ 0,0,500,50 };
	GSvector2 ubHPScale_{ 1,1 };

	//中ボス死亡ログ
	GSvector2 ubKillPosition_{ 690,150 };
	GSrect ubKillRect_{ 0,0,600,50 };
	GSvector2 ubKillScale_{ 1,1 };

	mutable GSrect thousand;
	mutable GSrect hundreds;
	mutable GSrect tens;
	mutable GSrect ones;

	//エンターキー
	GSrect enterRect_ = { 0,0,800,200 };
	GSvector2 enterScal_ = { 1.0f,1.0f };
	GSvector2 enterPos_ = { 600,100 };

};
#endif // !MISSION_H_