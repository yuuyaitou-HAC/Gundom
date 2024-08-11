#include "CameraTPS.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

//プレイヤーからの相対座標
const GSvector3 PlayerOffset{ 0.f,3.f,-5.f };
//カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.f,1.7f,0.f };

//コンストラクタ
CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at) {
	//ワールドを設定
	world_ = world;
	//タグの設定
	tag_ = "CameraTag";
	//名前の設定
	name_ = "Camera";
	//視点の位置を決定
	transform_.position(position);
	//注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
}

//更新
void CameraTPS::update(float delta_time) {
	//プレイヤーを検索
	Actor* player = world_->find_actor("Player");
	if (player == nullptr)return;
	//視点の位置を求める（プレイヤーの背後の座標）
	GSvector3 position = PlayerOffset * player->transform().localToWorldMatrix();
	//注視点の位置を求める（プレイヤーの頭部の少し上あたりの座標）
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	//
	Line line{ at,position };
	GSvector3 intersects;
	if (world_->field()->collide(line, &intersects)) {
		position = intersects;
	}

	//スムースダンプによる滑らかな補間
	const float SmoothTime{ 12.f }; //補間フレーム数
	const float MaxSpeed{ 1.f };    //移動スピードの最大値
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_,
		                             SmoothTime, MaxSpeed, delta_time);

	//視点の位置を設定
	transform_.position(position);
	//注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
}

//描画
void CameraTPS::draw()const {
	//視点の位置
	GSvector3 eye = transform_.position();
	//注視点の位置
	GSvector3 at = eye + transform_.forward();
	//カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, //視点の位置
		at.x, at.y, at.z,    //注視点の位置
		0.f, 1.f, 0.f);      //視点の上座標
}