#pragma once

#ifndef CAMEAR_FPS_H_
#define CAMEAR_FPS_H_

#include "Actor/Actor.h"

class CameraFPS:public Actor{
public:
	//コンストラクタ
	CameraFPS(IWorld* world);
	//描画
	virtual void draw()const override;
};


#endif // !CAMEAR_FPS_H_