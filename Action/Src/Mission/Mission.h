#ifndef MISSION_H_
#define MISSION_H_

#include "Actor/Actor.h"

#include "Player/Player.h"
#include "BOSS/Boss.h"
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

	virtual void draw() const override;

	virtual void draw_gui()const override;

private:

	void mission1(float delta_time);

	void mission2(float delta_time);

	void mission3(float delta_time);

	void mission4(float delta_time);



	void gameClear(float delta_time);

private:

	Player* player_;

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

};

#endif // !MISSION_H_
