#include "Camera/Camera.h"
#include "World/IWorld.h"

//
const GSvector3 PlayerOffset{ 0.f,3.f,-5.f };
//
const GSvector3 ReferencePointOffset{ 0.f,1.7f,0.f };

//コンストラクタ
Camera::Camera(IWorld* world) {
	world_ = world;
	name_ = "Camera";
	tag_ = "CameraTag";
}

//描画
void Camera::draw()const {

	//プレイヤーを検索
	Actor* player = world_->find_actor("Player");
	if (player == nullptr)return;
	//カメラの位置を求める
	GSvector3 eye = PlayerOffset * player->transform().localToWorldMatrix();
	//注視点の位置を求める
	GSvector3 at = player->transform().position() + ReferencePointOffset;

	//openGLのカメラ設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, //視点の位置
		at.x, at.y, at.z,    //注視点の位置
		0.f, 1.f, 0.f        //視点の上方向
	);
}
