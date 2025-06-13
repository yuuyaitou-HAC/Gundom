#ifndef ENEMY_SHIP_H_
#define ENEMY_SHIP_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "EnemyAI/TankAI.h"
#include "EnemyAI/HBMAI.h"
#include "Common/GameData.h"
#include "EnemyAI/EnemyAttackControl.h"

class EnemyShip : public Actor {
public:

	enum class MakeHBM {
		Gatling,
		BeamRifle,
		BeamSaber,
		Sniper
	};

	//コンストラクタ
	EnemyShip(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

private:

	void move(float delta_time);

	void makeAI(float delta_time);

	//ミッション1
	void mission1MakeAi();

	//戦車生成
	void makeTankAI();

	//HBM生成
	void makeHbmAI(EnemyShip::MakeHBM makehbm);

	//配列内で死んでいるものを調べる
	void diecheck();

private:

	AnimationMesh mesh_;

	Player* player_;

	GSuint motion_;

	MakeHBM makeHBM_;

	EnemyAttackControl* ebcontrol_;

	std::vector<TankAI*> tankais_;

	std::vector<HBMAI*> hbmais_;

	//各武器ごとの配列
	std::vector<HBMAI*> beamSaber_;
	std::vector<HBMAI*> gatring_;
	std::vector<HBMAI*> beamRifle_;
	std::vector<TankAI*> tank_;
private:

	//戦艦のサイズ
	const float enemyShipRadius_{ 0.8f };
	const float enemyShipHeight_{ 1.f };

	//生成時の高さ調整
	const float makeHight_{ 1.f };

	//各個体の現在の生成数
	int nowTank_ = 0;
	int nowGatling_ = 0;
	int nowBeamRifle_ = 0;
	int nowBeamSaber_ = 0;
	int nowSniper_ = 0;

	//カウントした敵の数
	int tankCounter_ = 0;
	int beamRifleCounter_ = 0;
	int gatringCounter_ = 0;

	//敵生成間隔
	float makeTimer_;

	float assignmentMakeTimer_ = 180.0f;

	//撤退終了したか
	bool retreatMission2Frag_ = false;

	//撤退終了したか
	bool retreatMission4Frag_ = false;

	//排除中かどうか
	bool finishRetreatFrag = false;

	//モーションループ
	bool motion_Loop_;

	//ボス登場フラグ
	bool bossFrag_;

	//ボスは生成したか
	bool bossMake_;

	//生成座標
	GSvector3 spawnPoint_;

	//自身の座標
	GSvector3 myPos_;

	float timeElapsed_ = 0.0f;
	GSvector3 basePosition_;

	// 上下の移動幅（最大±amplitudeだけ上下する）
	float amplitude_ = 0.05f;        // 上下の高さ
	float frequency_ = 0.005f;        // 周期（1秒で1往復）

	GSuint vernierEffect1_;
	GSuint vernierEffect2_;
	GSuint vernierEffect3_;

	GSvector3 vernierEffectPos1_{ -23,-2,0 };
	GSvector3 vernierEffectPos2_{ 10,-4,-5 };
	GSvector3 vernierEffectPos3_{ 10,-4,5 };
	GSvector3 vernierEffectEuler_{ 90,0,0 };
	GSvector3 vernierEffectScale_{ 3.f,3.f,2.5f };


	//地面の砂埃
	GSuint dustEffect_;
	GSvector3 dustEffectPos_;
	GSvector3 dustEffetEuler_ = GSvector3::zero();
	GSvector3 dustEffectScale_{ 20,20,20 };

	GScolor4 dustColor_{ 0.6f,0.6f, 0.6f, 1 };

	GSvector3 playerPos_;
	GSvector3 effectDrawPos_;

	//エフェクト用
	GSmatrix4 effectWorld_;
	GSmatrix4 localMatrix_;

	bool effectTrigger_;
};
#endif // !ENEMY_SHIP_H_