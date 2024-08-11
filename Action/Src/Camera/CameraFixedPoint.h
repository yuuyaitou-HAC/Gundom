#pragma once

#ifndef CAMERA_FIXED_POINT_H_
#define CAMERA_FIXED_POINT_H_

#include "Actor/Actor.h"

//カメラクラス
class CameraFixedPoint:public Actor{
public:
	//コンストラクタ
	CameraFixedPoint(IWorld* world);
	//描画
	virtual void draw()const override;
};

#endif;