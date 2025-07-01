#ifndef UNDER_BOSS_H_
#define UNDER_BOSS_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "UnderBossState.h"
#include "UnderBossGun/UnderBossGunController.h"

class Player;
class EnemyShip;

class UnderBoss : public Actor {

public:

	enum class State {
		Idle,		//アイドル
		Move,		//移動
		AttackMove,	//移動攻撃
		Slashing,	//斬撃
		Damage,		//ダメージ
		Baster,
		Retreat,	//退却
		Die,		//死
	};

public:

	//コンストラクタ
	UnderBoss(IWorld* world, const GSvector3& position);

	//デストラクタ
	~UnderBoss();

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//衝突判定
	virtual void react(Actor& other)override;

	//ボスステータス
	UnderBossState* underBossState_()const;


private:

	//銃の切り替え
	void changeGun();

	//状態の更新
	void update_state(float delta_time);

	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);

	//移動
	void move(float delta_time);

	//飛ぶかどうか
	void changeFly();

	//飛ぶ
	void fry(float delta_time);

	//移動攻撃
	void attackMove(float delta_time);

	//移動攻撃時の移動ポイント
	GSvector3 attackPoint();

	bool onTheLine(GSvector3 point)const;

	//ターゲットとの角度
	float target_signed_angle(GSvector3 target);

	//ターゲットとの距離
	float target_distance(GSvector3 Targetpos, GSvector3 pos);

	//攻撃中
	void shoot(float delta_time);

	void baster(float delta_time);

	//斬撃
	void slash(float delta_time);

	//後退
	void afterAlash();

	void retreat(float delta_time);

	//ダメージ中
	void damage(float delta_time);

	void death(float delta_time);

	//ターゲットの方向を向く
	void faceThePlayer(float delta_time);

	//フィールドとの衝突判定
	void collide_field();

	//アクターとの衝突判定
	void collide_actor(Actor& other);

	//+か-を渡す
	int sign();

private:

	//アニメーションメッシュ
	AnimationMesh mesh_;

	//モーション番号
	GSuint motion_;

	//状態
	State state_{ State::Move };

	//ボスのステータスクラス
	UnderBossState* underbossstate_{ NULL };

	//ボス弾管理クラス
	UnderBossGunController* GC_{ NULL };

	//プレイヤー
	Player* player_{ NULL };

	EnemyShip* enemyship_{NULL};

private:

	//斬撃の攻撃量
	const int slashValue_{ 10 };

	//ボスの高さ
	const float underBossHeight_{ 1.5f };

	//衝突判定用の半径
	const float underBossRadius_{ 1.5f };

	//重力
	const float gravity_{ -0.016f };

	//足元のオフセット
	const float footOffset_{ 0.1f };

	//振り返るときの速度
	const float turnAngle_{ 2.5f };

	//走るときの速さ
	const float runSpeed_{ 2.0f };

	//ある程度の誤差を強要するための閾値
	const double EPSILON_{ 1e-9 };

	//受けたダメージ量
	int damage_{ 0 };

	//状態タイマ
	float stateTimer_{ 0.0f };

	//次の移動までの間隔
	float moveTimer_{ 0.0f };

	//次の移動までの間隔(代入)
	const float assignmentMoveTimer_{ 60.0f };

	//次の飛ぶ場所指定までの時間
	float fryTimer_{ 0.0f };

	//次の飛ぶ時間までの時間(代入)
	const float assignmentFryTimer_{ 60.0f };

	//移動速度
	float walkSpeed_{ 0.0f };

	//射撃間隔
	float shootTime_{ 20.0f };
	const float assignmentBeamShootTime_{ 20.0f };
	const float assignmentGatringShootTime_{ 5.0f };

	//斬撃の調整前方
	const float slashDistance_{ 1.5f };

	//斬撃の高さ
	const float slashHight_{ 1.0f };

	//銃切り替えの距離
	const float weaponDistance_{ 10.0f };

	//バスターライフル発射までの時間
	float basterTimer_{ 120.0f };

	//無敵時間
	float invincibleTimer_{ 120.0f };
	const float assignmnetInvincibleTimer_{ 120.0f };

	//ダメージ時に半透明にするための値
	float meshAlpha_{ 1.0f };
	
	//モーションのループ指定
	bool motionLoop_{ true };

	//飛ぶか
	bool isFry_{ false };
		
	//ダメージ直後かどうか
	bool damageFrag_{ false };

	//自身の座標
	GSvector3 myPos_{ GSvector3().zero() };

	//自身の回転
	GSvector3 rotate_{ GSvector3().zero() };

	//プレイヤーの座標
	GSvector3 playerPos_{ GSvector3().zero() };

	GSvector3 point_{ GSvector3().zero() };

	//移動攻撃のポイント
	GSvector3 attackPoint_{ GSvector3().zero() };

	//飛んでいるときの向かう座標
	GSvector3 fryPow_{ GSvector3().zero() };

	//飛ぶ高さのランダム
	GSvector2 fryRand_{ 0,10 };

	float maxSpeed_{ 0.0f };

	//慣性用のスピード変数
	float speed_{ 0.0f };

	//過去の方向ベクトル
	GSvector3 postmoveTo_{ GSvector3().zero() };

	//向かう方向
	GSvector3 moveTo_{ GSvector3().zero() };

	//ヒットエフェクト
	GSuint effectHit_;
	bool drawmeshFrag_{ true };

	//増減
	bool fluctuation_{ false };

	bool movein_{ false };

	float reductionRate_{ 0.0f };

	float reduceSpeed_{ 0.0f };

	GSuint hiteffect_;

	//爆発エフェクト再生したかどうか
	bool playExplosionEffect_{ false };

	//爆発エフェクト
	GSuint effectExplosionL_;

};
#endif // !BOSS_H_