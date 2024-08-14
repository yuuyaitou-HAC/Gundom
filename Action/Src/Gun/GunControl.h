#ifndef GUN_CONTROL_H_
#define GUN_CONTROL_H_

#include "Actor/Actor.h"
#include <gslib.h>
#include "BeamGun.h"

//class BeamGun;

class GunControl :public Actor {

public:
	//弾の種類
	enum class Bullet {
		Beamlifl,	//通常弾
		BeamMagnum,	//ビームマグナム
		Bazooka,	//バズーカ
	};

public:

	//コンストラクタ
	GunControl(IWorld* world, const GSvector3& position);
	~GunControl();

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw() const override;

	//guiの描画
	virtual void draw_gui() const override;

	//衝突リアクション
	virtual void react(Actor& other)override;

	//弾の種類を変える
	void ChangeState();

	//ステータスに応じて弾を撃つ
	void Fire();

	BeamGun* BG() const;

private:

	BeamGun* bg;

	//ステータス
	Bullet bullet = Bullet::Beamlifl;
	
};

#endif // !GUN_CONTROL_H_
