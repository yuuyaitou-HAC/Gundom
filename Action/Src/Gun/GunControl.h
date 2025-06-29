#ifndef GUN_CONTROL_H_
#define GUN_CONTROL_H_

#include "Actor/Actor.h"
#include <gslib.h>
#include "BeamGun.h"
#include "BeamMagnum.h"
#include "Bazooka.h"

class GunControl :public Actor {

public:
	//弾の種類
	enum class Bullet {
		Beamlifl,	//通常弾
		BeamMagnumBullet,	//ビームマグナム
		BazookaBullet,	//バズーカ
	};

public:

	//コンストラクタ
	GunControl(IWorld* world, const GSvector3& position);
	~GunControl();

	//更新
	virtual void update(float delta_time)override;

	//弾の種類を変える
	void ChangeState(float delta_time);

	//ステータスに応じて弾を撃つ
	void Fire();

private:

	//ビームライフル
	BeamGun* bg_{ NULL };

	//ビームマグナム
	BeamMagnum* bm_{ NULL };

	//バズーカ
	Bazooka* bz_{ NULL };

	//プレイヤー
	Player* player_{ NULL };

	//ステータス
	Bullet bullet_ = Bullet::Beamlifl;

	//マウスホイールの動き
	int mouseZ_{ 0 };

	//過去のステータス
	int stateNum_{ 0 };

	//現在のステータス
	int nowStateNum_{ 0 };

	float changeCollTimer_{ 30.0f };
	const float assignmentChangeCollTimer_{ 30.0f };
};
#endif // !GUN_CONTROL_H_