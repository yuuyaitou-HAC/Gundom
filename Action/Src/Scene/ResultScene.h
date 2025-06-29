#ifndef RESULT_SCENE_H_
#define RESULT_SCENE_H_

#include "Common/GameData.h"
#include "World/World.h"
#include "Actor/Actor.h"
#include <vector>
#include <string>

class ResultScene : public Actor {

public:

	ResultScene(World* world);

	void update(float delta_time);

	void draw() const;

private:
	mutable int ResultScore{0};


	//テクスチャ用
	GScolor4 textureColor_{ 256,256,256,1.0f };



	//リザルト背景
	GSvector2 resultBasePos_{ 0,0 };
	GSrect resultBaseRect_{ 0,0,1920,1080 };
	GSvector2 resultBaseScale_{ 1,1 };

	//ゲームの結果
	GSvector2 gameRisultPos_{ 785,70 };
	GSrect gameRisult1Rect_{ 0,0,350,60 };
	GSrect gameRisult2Rect_{ 0,60,350,120 };
	GSvector2 gameRisultScale_{ 1,1 };

	//ミッション
	GSvector2 MissionResult1Pos_{ 900,160 };
	GSvector2 MissionResult2Pos_{ 900,220 };
	GSvector2 MissionResult3Pos_{ 900,280 };
	GSvector2 MissionResult4Pos_{ 900,340 };
	GSrect MissionResult1Rect_{ 0,0,350,60 };
	GSrect MissionResult2Rect_{ 0,60,350,120 };
	GSvector2 MissionResultScale_{ 0.5,0.5 };


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

	mutable GSrect tens;
	mutable GSrect ichi;

	mutable GSvector2 evaluationPos_{ 885,800 };
	GSvector2 evaluationScale_{ 0.6,0.6 };
	GSrect evaluationRect_[5]{
	GSrect{0,  0, 350,  350},
	GSrect{0, 350, 350, 700},
	GSrect{0,700, 350, 1050},
	GSrect{0,1050, 350, 1400},
	GSrect{0,1400, 350, 1750}
	};
};
#endif