#ifndef BAZOOKA_H_
#define BAZOOKA_H_

#include "Actor\Actor.h"
#include "Actor\AnimationMesh.h"
#include <gslib.h>

class Player;

class Bazooka :public Actor {

public:

	//コンストラクタ
	Bazooka(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw() const override;

	//guiの描画
	virtual void draw_gui() const override;

	//衝突リアクション
	virtual void react(Actor& other)override;

	void Fire();


private:

	Player* player_;

};

#endif // !BEAM_MAGNUM_H_
