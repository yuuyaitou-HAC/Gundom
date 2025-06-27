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

	//武器の種類
	enum class MakeHBMWeapon {
		BeamSaber,
		Gatling,
		BeamRifle,
		Sniper
	};

	//コンストラクタ
	EnemyShip(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

private:

	//エフェクトの更新
	void effect_update();

	//移動
	void move(float delta_time);

	//AI生成
	void make_AI(float delta_time);

	//戦車生成
	void make_tankAI();

	//HBM生成
	void make_hbmAI(EnemyShip::MakeHBMWeapon makehbm);

	//配列内で死んでいるものを調べる
	void die_check();

private:

	//メッシュ
	AnimationMesh mesh_;

	//プレイヤー
	Player* player_{ NULL };

	//敵の攻撃管理クラス
	EnemyAttackControl* ebcontrol_{ NULL };

	//各個体の現在の生成数
	int nowTank_{ 0 };
	int nowGatling_{ 0 };
	int nowBeamRifle_{ 0 };
	int nowBeamSaber_{ 0 };
	int nowSniper_{ 0 };

	//カウントした敵の数
	int tankCounter_{ 0 };
	int beamRifleCounter_{ 0 };
	int gatlingCounter_{ 0 };

	//各部隊の最低生成数
	const int minMakeTank_{ 3 };
	const int minMakeGatling_{ 1 };
	const int minMakeBeamSaber_{ 1 };
	const int minMakeBeamRifle_{ 3 };
	const int minMakeSniper_{ 1 };

	//各部隊の最大生成数
	const int maxMakeTank_{ 5 };
	const int maxMakeGatling_{ 2 };
	const int maxMakeBeamRifle_{ 5 };

	//各部隊の構成人数
	const unsigned int beamSaberUnitNum_{ 3 };
	const unsigned int gatlingUnitNum_{ 3 };
	const unsigned int BeamRifleUnitNum_{ 5 };
	const unsigned int SniperUnitNum_{ 3 };

	//スナイパー生成するための距離
	const float sniperMakeDistnace_{ 50 };

	//戦艦のサイズ
	const float enemyShipRadius_{ 0.8f };
	const float enemyShipHeight_{ 1.f };

	//生成時の高さ調整
	const float makeHeight_{ 1.f };

	//敵生成間隔
	float makeTimer_{ 0.0f };

	//敵生成間隔(代入)
	const float assignmentMakeTimer_{ 180.0f };

	//砂埃の描画座標
	const float dustEffectposY_{ -8.0f };

	//れいの長さ
	const float rayLength_{ 30.0f };

	float timeElapsed_{ 0.0f };

	// 上下の移動幅（最大±amplitudeだけ上下する）
	const float amplitude_{ 0.05f };        // 上下の高さ
	const float frequency_{ 0.005f };        // 周期（1秒で1往復）

	//プレイヤーとの距離
	float playerDistance_{ 0.0f };

	//撤退終了したか
	bool retreatMission2Frag_{ false };

	//撤退終了したか
	bool retreatMission4Frag_{ false };

	//排除中かどうか
	bool finishRetreatFrag{ false };

	//エフェクト再生するかどうか
	bool isDrawEffect_{ true };

	//生成座標
	GSvector3 spawnPoint_{ GSvector3().zero() };

	//自身の座標
	GSvector3 myPos_{ GSvector3().zero() };

	//ボスの生成場所
	const GSvector3 bossMakePos_{ -200,10,1.5 };

	//戦車AI
	std::vector<TankAI*> tankais_{ NULL };

	//HBMAI
	std::vector<HBMAI*> hbmais_{ NULL };

	//各武器ごとの配列
	std::vector<HBMAI*> beamSaber_{ NULL };
	std::vector<HBMAI*> gatling_{ NULL };
	std::vector<HBMAI*> beamRifle_{ NULL };
	std::vector<TankAI*> tank_{ NULL };

	//バーニアエフェクト関係
	GSuint vernierEffect1_{ 0 };
	GSuint vernierEffect2_{ 0 };
	GSuint vernierEffect3_{ 0 };

	const GSvector3 vernierEffectPos1_{ -23,-2,0 };
	const GSvector3 vernierEffectPos2_{ 10,-4,-5 };
	const GSvector3 vernierEffectPos3_{ 10,-4,5 };
	const GSvector3 vernierEffectEuler_{ 90,0,0 };
	const GSvector3 vernierEffectScale_{ 3.f,3.f,2.5f };


	//地面の砂埃
	GSuint dustEffect_{ 0 };
	GSvector3 dustEffectPos_{ GSvector3().zero() };
	GSvector3 dustEffetEuler_{ GSvector3::zero() };
	const GSvector3 dustEffectScale_{ 20,20,20 };

	//エフェクトを再生する範囲
	const GSvector2 effectDrawDistance_{ 10,100 };

	const GScolor4 dustColor_{ 0.6f,0.6f, 0.6f, 1 };

	GSvector3 playerPos_{ GSvector3().zero() };
	GSvector3 effectDrawPos_{ GSvector3().zero() };

	//エフェクト用
	GSmatrix4 effectWorld_;
	GSmatrix4 localMatrix_;
};
#endif // !ENEMY_SHIP_H_