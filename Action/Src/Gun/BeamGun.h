#ifndef BEAMUGUN_H_
#define BEAMUGUN_H_

#include "Actor\Actor.h"
#include "Actor\AnimationMesh.h"
#include <gslib.h>

class Player;

class GunControl;

class BeamGun :public Actor {

public:

	//コンストラクタ
	BeamGun(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//発砲
	void Fire();

private:

	//クールタイム
	void Cool(float delta_time);

private:

	//プレイヤー
	Player* player_{ NULL };

	GunControl* gunControl_{ NULL };

private:

	//残弾数
	int nowMagazine_{ 0 };

	//残弾数(代入)
	int BazookaMagazine_{ 0 };

	//クールタイム
	float coolTime_{ 240.0f };

	//クールタイム(代入)
	float assignmentCoolTime_{ 240.0f };

	//クールタイム中か
	bool coolTimeTriger_{ false };

	//弾を生成する場所の距離
	float GenerateDistance{ 1.8f };

	//生成する位置の高さの補正値 1.5
	float GenerateHeight{ 1.7f };

	//弾の移動スピード
	float Speed{ 1.f };
};
#endif // !BEAMUGUN_H_