#pragma once

#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include"IScene.h"
#include <gslib.h>
#include "LoadScene.h"

//タイトルシーン
class TitleScene:public IScene{
public:
	//開始
	virtual void start()override;
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//終了しているか？
	virtual bool is_end()const override;
	//次のシーンを返す
	virtual std::string next()const override;
	//終了
	virtual void end()override;

private:

	LoadScene load_;

	//背景の描画
	void draw_background(GSuint id)const;
	
	bool is_load_;

	//終了フラグ
	bool is_end_{ false };

	GSvector2 pos_T = { 1450.0f,950.0f };
	GSrect rect_T = { 0,0,256,128 };
	mutable GScolor color_T = { 1.0f,1.0f,1.0f,1.0f };
	GSvector2 scal_T = { 1.0f,1.0f };
	mutable GSfloat Alpha = 0.0f;

	GSfloat delta_timer;

	float M_PI = 3.14159265f;

};

#endif // !TITLE_SCENE_H_