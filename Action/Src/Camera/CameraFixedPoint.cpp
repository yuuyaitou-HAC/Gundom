#include "Camera/CameraFixedPoint.h"


//コンストラクタ
CameraFixedPoint::CameraFixedPoint(IWorld* world) {
	//ワールドを設定
	world_ = world;
	//タグの設定
	tag_ = "CameraTag";
	//名前の設定
	name_ = "Camera";
}

//描画
void CameraFixedPoint::draw()const {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		10.f, 10.f, 10.f,
		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f
	);
}