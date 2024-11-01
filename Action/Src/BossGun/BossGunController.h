#ifndef BOSS_GUNCONTROLLER_H_
#define BOSS_GUNCONTROLLER_H_

#include "Actor/Actor.h"
#include <gslib.h>
#include "BossBeamRifle.h"
#include "Gatling.h"
#include "BossBasterRifle.h"

class BossGunController{
public:

	//銃の種類
	enum class GunState
	{
		Beamlifl,
		Gatling,
		Basterlifl,
	};

public:

	//コンストラクタ
	BossGunController(IWorld* world, const GSvector3& position);
	
	//デストラクタ
	~BossGunController();

	//更新
	virtual void update(float delta_time);

	//ステータスの変更
	virtual void changeState();

	//描画
	virtual void draw() const;

	//ステータスの変更(ボス側から)
	void SetState(int num);

	//発砲
	void Fire();

private:

	BossBeamRifle* BR;
	Gatling* G;
	BossBasterRifle* BS;
	GunState gunstate;

private:
	int GunNum;

	float Fire_timer;

	IWorld* world_;

	GStransform transform_;

};



#endif // !BOSS_GUNCONTROLLER_H_
