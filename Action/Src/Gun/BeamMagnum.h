#ifndef BEAM_MAGNUM_H_
#define BEAM_MAGNUM_H_

#include "Actor\Actor.h"
#include "Actor\AnimationMesh.h"
#include <gslib.h>

class Player;

class BeamMagnum :public Actor {

public:

	//コンストラクタ
	BeamMagnum(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw() const override;

	//guiの描画
	virtual void draw_gui() const override;

	//衝突リアクション
	virtual void react(Actor& other)override;

	void Fire();

	int test();

private:

	Player* player_;

private:

	//残弾数
	int NowMagazine;

	//残弾数(代入)
	int AsignmentMagazine;

	float CoolTime;

	float AsignmentCoolTime;

	bool CoolTimeTriger;

	float delta_timer;

	void Cool();

	int Magazin;

};

#endif // !BEAM_MAGNUM_H_
