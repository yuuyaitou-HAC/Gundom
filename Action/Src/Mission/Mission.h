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

	//HPバー(青)
	GSvector2 HPposition{ 700,100 };
	GSrect HPRect{ 0,0,500,20 };
	GSvector2 HPScale{ 1,1 };
	GScolor4 HPColor{ 256,256,256,1.0f };

	//HPバー(灰)
	GSvector2 HPBackposition{ 1200,120 };
	GSrect HPBackRect{ 0,0,500,20 };
	GScolor4 HPBackColor{ 256,256,256,1.0f };
	float HPBarScale;

};
#endif // !MISSION_H_