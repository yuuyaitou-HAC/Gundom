#ifndef BOSS_GUNCONTROLLER_H_
#define BOSS_GUNCONTROLLER_H_

#include "Actor/Actor.h"
#include <gslib.h>
#include "UnderBossBeamRifle.h"
#include "Gatling.h"
#include "UnderBossBasterRifle.h"

class UnderBossGunController {
public:

	//銃の種類
	enum class GunState
	{
		Beamlifl,
		Gatling,
		Basterlifl,
	};

public:

	UnderBossGunController(const UnderBossGunController&) = delete;
	UnderBossGunController& operator=(const UnderBossGunController&) = delete;

	//コンストラクタ
	UnderBossGunController(IWorld* world, const GSvector3& position, const UnderBoss* underBoss);

	//デストラクタ
	~UnderBossGunController();

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

	UnderBossBeamRifle* beamLifle_;
	Gatling* gatling_;
	UnderBossBasterRifle* basterLifle_;
	GunState gunState_{ GunState::Beamlifl };

private:
	int gunNum_{ 1 };

	float fireTimer_{ 10.0f };
	const float assignmentFireTimer_{ 10.0f };

	IWorld* world_;

	GStransform transform_;
};
#endif // !BOSS_GUNCONTROLLER_H_