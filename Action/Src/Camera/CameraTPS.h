#ifndef CAMERA_TPF_H_
#define CAMERA_TPF_H_

#include "Actor/Actor.h"

class CameraTPS :public Actor {
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
	float yae_{ 0.0f };

	//プレイヤーオフセットのZ値
	float playerOffsetZ_;

	//プレイヤーオフセットのY値
	float playerOffsetY_{5.5f};
	
	//LeapのA値
	float leapA_;
	
	//プレイヤー
	Actor* player_{ nullptr };

	GSvector3 playerOffset_;

	//上下のカメラ感度
	float cameraSensitivity_{0.1f};

	//カメラトプレイヤーの距離間の調整
	float dashDistance_{-9.0f};
	float nomalDistance_{-6.0f};

	//プレイヤーオフセット用
		//スムースダンプによる滑らかな補間
	float playerOffsetSmoothTime_{ 12.0f }; //補間フレーム数
	float playerOffsetMaxSpeed_{ 10.5f };    //移動スピードの最大値

	//スムースダンプによる滑らかな補間
	const float smoothTime_{ 12.0f }; //補間フレーム数
	const float maxSpeed_{ 10.5f };    //移動スピードの最大値

	//カメラの注視点の補正値 2.0
	const GSvector3 referencePointOffset_{ 0.f,3.0f,0.f };
};
#endif // !CAMERA_TPF_H_