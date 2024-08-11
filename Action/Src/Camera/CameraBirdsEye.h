#pragma once

#ifndef CAMERA_BIRDS_EYE_H_
#define CAMERA_BIRDS_EYE_H_

#include "Actor/Actor.h"

//俯瞰カメラクラス
class CameraBirdsEye:public Actor{
public:
	//コンストラクタ
	CameraBirdsEye(IWorld* world);
	//描画
	virtual void draw()const override;
};


#endif