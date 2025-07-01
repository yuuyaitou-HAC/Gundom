#ifndef PLAYER_AHIP_H_
#define PLAYER_AHIP_H_

#include "Actor/Actor.h"
#include "Common/GameData.h"
#include "string"
#include "Player/Player.h"

class PlayerShip : public Actor {
public:

	//コンストラクタ
	PlayerShip(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

private:

	//エフェクトの更新
	void effect_update();

	//移動
	void move(float delta_time);

private:

	AnimationMesh mesh_;
	Player* player_{ NULL };

	float timeElapsed_{ 0.0f };

	const float amplitude_{ 0.05f };
	const float frequency_{ 0.005f };

	//砂埃の描画座標
	const float dustEffectposY_{ -8.0f };

	const float PlayerShipRadius_{ 0.8f };
	const float PlayerShipHeight_{ 1.f };

	//プレイヤーとの距離
	float playerDistance_{ 0.0f };

	//エフェクト再生するかどうか
	bool isDrawEffect_{ true };

	//プレイヤー座標
	GSvector3 playerPos_{ GSvector3().zero() };

	GSvector3 effectDrawPos_{ GSvector3().zero() };

	//自身の座標
	GSvector3 myPos_{ GSvector3().zero() };

	//エフェクトを再生する範囲
	const GSvector2 effectDrawDistance_{ 10,100 };

	//地面の砂埃
	GSuint dustEffect_{ 0 };
	GSvector3 dustEffectPos_{ GSvector3().zero() };
	GSvector3 dustEffectEuler_{ GSvector3().zero() };
	const GSvector3 dustEffectScale_{ 20,20,20 };
	const GScolor4 dustEffectColor_{ 0.6, 0.6, 0.6, 1 };

	//エフェクト用
	GSmatrix4 effectWorld_;
	GSmatrix4 localMatrix_;

	//バーニア系のエフェクト
	GSuint vernierEffect1_{ 0 };
	GSuint vernierEffect2_{ 0 };
	GSuint vernierEffect3_{ 0 };

	const GSvector3 vernierEffectPos1_{ -23,-2,0 };
	const GSvector3 vernierEffectPos2_{ 10,-4,-5 };
	const GSvector3 vernierEffectPos3_{ 10,-4,5 };
	const GSvector3 vernierEffectEuler_{ 90,0,0 };
	const GSvector3 vernierEffectScale_{ 3,3,2.5 };
};
#endif // !PLAYER_AHIP_H_