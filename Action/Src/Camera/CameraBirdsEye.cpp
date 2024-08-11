#include "CameraBirdsEye.h"
#include "World/IWorld.h"

//プレイヤーからのオフセット
const GSvector3 PlayerOffset{ 0.f,15.f,-2.f };

//コンストラクタ
CameraBirdsEye::CameraBirdsEye(IWorld* world) {
	//ワールドの設定
	world_ = world;
	//タグの設定
	tag_ = "CameraTag";
	//名前の設定
	name_ = "Camera";
}

//描画
void CameraBirdsEye::draw()const {
	//プレイヤーの検索
	Actor* player = world_->find_actor("Player");
	if (player == nullptr)return;

	//視点の位置
	GSvector3 eye = player->transform().position() + PlayerOffset;
	//注視点の位置
	GSvector3 at = player->transform().position();

	//カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,
		at.x, at.y, at.z,
		0.f, 0.f, 1.f
	);
}