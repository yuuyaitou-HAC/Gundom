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
	mutable int ResultScore;


	//テクスチャ用

	//リザルト背景
	GSvector2 resultBasePos_{ 0,0 };
	GSrect resultBaseRect_{ 0,0,1920,1080 };
	GSvector2 resultBaseScale_{ 1,1 };
	GScolor4 resultBaseColor_{ 256,256,256,1.0f };

	//ゲームの結果
	GSvector2 gameRisultPos_{ 785,70 };
	GSrect gameRisult1Rect_{ 0,0,350,60 };
	GSrect gameRisult2Rect_{ 0,60,350,120 };
	GSvector2 gameRisultScale_{ 1,1 };
	GScolor4 gameRisultColor_{ 256,256,256,1.0f };

	//ミッション
	//GSvector2 ResultPos_{ 0,0 };
	//GSvector2 ResultPos_{ 0,0 };
	//GSvector2 ResultPos_{ 0,0 };
	//GSvector2 ResultPos_{ 0,0 };
	//GSrect ResultRect_{ 0,0,1920,1080 };
	//GSrect ResultRect_{ 0,0,1920,1080 };
	//GSvector2 ResultScale_{ 1,1 };
	//GScolor4 ResultColor_{ 256,256,256,1.0f };

	


};

#endif // !RESULT_SCENE_H_