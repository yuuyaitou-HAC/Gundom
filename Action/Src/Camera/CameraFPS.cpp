#include "CameraFPS.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

//プレイヤーの座標からのオフセット
const GSvector3 PlayerOffset{ 0.f,1.8f,-0.1f };

//コンストラクタ
CameraFPS::CameraFPS(IWorld* world) {
	//ワールドを設定
	world_ = world;
	//タグの設定
	tag_ = "CameraTag";
	//名前の設定
	name_ = "Camera";
}

//描画
void CameraFPS::draw()const {
	//プレイヤーを検索
	Actor* player = world_->find_actor("Player");
	if (player == nullptr)return;

	//視点の位置
	GSvector3 eye = PlayerOffset * player->transform().localToWorldMatrix();
	//注視点の位置
	GSvector3 at = eye + player->transform().forward();

	//カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, //視点の位置
		at.x, at.y, at.z,    //注視点の位置
		0.f, 1.f, 0.f);      //視点の方向
}