#pragma once

#ifndef CAMERA_TPF_H_
#define CAMERA_TPF_H_

#include "Actor/Actor.h"

class CameraTPS:public Actor{
public:
	//コンストラクタ
	CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;

	GSvector3 ScreenToWorldPoint(float screenX, float screenY, float depth, GSmatrix4& prog, GSmatrix4& diew, GSvector2 screensize);

private:

	//x軸回りの回転角度
	float camerapich_;
	float yae{ 0.0f };
	//プレイヤーオフセットのZ値
	float PlayerOffsetZ;
	//LeapのA値
	float LeapA;

	Actor* player{ nullptr };



};

#endif // !CAMERA_TPF_H_