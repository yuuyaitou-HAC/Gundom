#include "CameraTPS.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

//コンストラクタ
CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at) :
	camerapich_{ 0.0f },
	playerOffsetZ_{ 0.0f },
	leapA_{ 0.0f }
{
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
	//x軸回りの回転角度の初期化
	camerapich_ = (at - position).getPitch();
}

//更新
void CameraTPS::update(float delta_time) {
	player_ = world_->find_actor("Player");
	if (player_ == nullptr)return;

	//プレイヤーからの相対座標
	GSvector3 PlayerOffset;
	playerOffsetZ_ = LERP(leapA_, -6.0f, -8.0f);
	leapA_ = CLAMP(leapA_, 0.0f, 1.0f);
	float Attackresult{ 0.2f };
	if ((gsGetKeyState(GKEY_LSHIFT) && !gsGetMouseButtonState(GMOUSE_BUTTON_1)) || gsGetKeyState(GKEY_SPACE)) {
		//ダッシュ時又はジャンプ中など
		leapA_ += 0.1f;
		PlayerOffset = { 0.f,playerOffsetY_,playerOffsetZ_ + dashDistance_ };
	}
	else {
		//通常時
		leapA_ -= 0.1f;
		PlayerOffset = { 0.f,playerOffsetY_,playerOffsetZ_ + nomalDistance_ };
	}

	//カメラの前後の動きを補完
	playerOffset_ = GSvector3::smoothDamp(playerOffset_, PlayerOffset, velocity_,
		playerOffsetSmoothTime_, playerOffsetMaxSpeed_, delta_time);

	//視点の位置を求める
	GSvector3 position = playerOffset_ * player_->transform().localToWorldMatrix();
	//注視店の位置を求める
	GSvector3 at = player_->transform().position() + referencePointOffset_;

	// マウスでカメラの上下移動
	int mx, my, mz;
	gsGetMouseVelocity(&mx, &my, &mz);
	camerapich_ += (float)my * cameraSensitivity_ * delta_time;

	camerapich_ = CLAMP(camerapich_, -55.0f, 25.0f);
	GSquaternion e = GSquaternion::euler(camerapich_, 0.0f, 0.0f);
	position = e * playerOffset_ * player_->transform().localToWorldMatrix();

	// 初期カメラ判定
	Line line{ at,position };
	GSvector3 intersects;
	if (world_->field()->collide(line, &intersects)) {
		position = intersects;
	}

	//ここで補正されている
	GSvector3 movePosition = GSvector3::smoothDamp(transform_.position(), position, velocity_,
		smoothTime_, maxSpeed_, delta_time);

	position.y = movePosition.y;

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

GSvector3 CameraTPS::ScreenToWorldPoint(float screenX, float screenY, float depth, GSmatrix4& prog, GSmatrix4& diew, GSvector2 screensize)
{
	float normalizedX = (2.0f * screenX / screensize.x) - 1.0f;
	float normalizedY = 1.0f - (2.0f * screenY / screensize.y);
	float normalizedZ = (2.0f * depth) - 1.0f;

	GSmatrix4 proj = prog.getInverse();
	GSmatrix4 View = diew.getInverse();

	GSvector4 clip = proj.transform(GSvector4(normalizedX, normalizedY, normalizedZ, 1.0f));
	GSvector4 viewspace = GSvector4(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w, 1.0f);

	GSvector3 worldPos = View.transform(viewspace).xyz;

	return worldPos;
}