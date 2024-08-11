#pragma once

#ifndef CAMERA_ROTATE_AROUND_H_
#define CAMERA_ROTATE_AROUND_H_

#include "Actor/Actor.h"

//三人称カメラクラス
class CameraRotateAround:public Actor{
public:
	//コンストラクタ
	CameraRotateAround(IWorld* world, const GSvector3& position, const GSvector3& at);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;

private:
	//y軸周りの回転速度
	float yaw_{ 0.f };
	//x軸周りの回転速度
	float pitch_{ 0.f };
};

#endif // !CAMERA_ROTATE_AROUND_H_