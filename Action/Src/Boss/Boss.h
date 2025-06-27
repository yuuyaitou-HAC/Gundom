#ifndef Boss_H_
#define Boss_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Boss/BossState.h"

class Player;

class EnemyShip;

class Boss : public Actor {

public:

	//状態
	enum State {
		FirstMove,	//初めの移動
		//Move,		//移動
		AttackMove,	//攻撃移動
		//FryAttack,	//飛び攻撃
		Cleaver,	//薙ぎ払い
		FireBullet,	//弾発射
		Damage,		//ダメージ状態
		Die,		//死亡
	};

public:

	//コンストラクタ
	Boss(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.f,0.f,0.f });

	//デストラクタ
	~Boss();

	//更新
	void update(float delta_time)override;

	//描画
	void draw() const override;

	//当たり判定
	virtual void react(Actor& other)override;

	//死亡判定
	bool die_trigger()const;

public:

	//ボスステータス
	BossState* boss_state() const;

private:

	//ステータス更新
	void update_state(float delta_time);

	//ステータス変更
	void change_state(State state, GSuint motion, bool loop = true);

	//初めの移動
	void first_move(float delta_time);

	//移動攻撃
	void attack_move(float delta_time);

	//薙ぎ払い
	void cleaver(float delta_time);

	//ダメージ
	void damage(float delta_time);

	//死亡
	void die(float delta_time);

	//射撃
	void bullet_fire(float delta_time);

	//ミサイル
	void fire_missile(float delta_time);

	//対象の方向を向かせる
	void face_the_target(GSvector3 target, float delta_time);

	//ターゲットとの角度を符号付きで返す
	float target_signed_angle(GSvector3 target);

	//フィールドとの衝突判定
	void collide_field();
	//アクターとの衝突判定
	void collide_actor(Actor& other);

private:

	//アニメーションメッシュ
	AnimationMesh mesh_;

	//モーション番号
	GSuint motion_{ 12 };

	//状態
	State state_{ State::FirstMove };

	//ボスのステータス
	BossState* bossstate_{ nullptr };

	//プレイヤー
	Player* player_{ nullptr };

	//自陣の戦艦
	EnemyShip* enemyShip_{ nullptr };

	//受けたダメージ
	int damageValue_{ 0 };

	//移動のランダム範囲
	int randPos_{ 50 };

	//爆発エフェクトの描画座標
	int randEffectPos_{ 5 };

	//ミサイル生成数
	int makeMissileNum_{ 5 };

	//ボスの高さ
	const float bossHeight_{ 4.0f };

	//衝突判定用の半径
	const float bossRadius_{ 3.5f };

	//振り返るときの速度
	const float turnAngle_{ 2.5f };

	//重力
	const float gravity_{ -0.016f };

	//足元のオフセット
	const float footOffSet_{ 0.1f };

	//状態タイマ
	float stateTimer_{ 0.0f };

	//移動速度
	float walkSpeed_{ 0.0f };

	//射撃クールタイム
	float beamFireCoolTime_{ 180.0f };
	const float assignmentBeamFireCoolTime_{ 180.0f };

	//射撃継続時間
	float beamFireTime_{ 240.0f };
	const float assignmentBeamFireTime_{ 240.0f };

	//射撃間の時間
	float fireInterval_{ 30.0f };
	const float assignmentFireInterval_{ 30.0f };

	//ミサイルクールタイム
	float missileCoolTime_{ 300.0f };
	const float assignmentMissileCoolTime_{ 300.0f };

	//次の爆破エフェクト再生までの時間
	float explosionTimer_{ 5.0f };
	const float assignmentExplosionTimer_{ 5.0f };

	//無敵時間
	float invincibleTimer_{ 120.0f };
	const float assignmentInvincibleTimer_{ 120.0f };

	//ダメージ時に半透明にするための値
	float meshAlpha_{ 1.0f };

	//初めの移動終了までの距離
	const float firstMoveFinishDistance_{ 2.0f };

	//ダメージ時のα値
	const float damageAlpha_{ 0.5f };

	//通常時のα値
	const float nomalAlpha_{ 1.0f };

	//目標地点更新距離
	const float updateRandPos_{ 5.0f };

	//斬撃発動距離
	const float cleaverDistance_{ 8.0f };

	//斬撃生成位置の調整
	const float makeCleaverPosOffset_{ 7.0f };

	//斬撃の大きさ
	const float slashRadius_{ 2.5f };

	//ランダム移動先に移動し終えたかどうか
	bool randMoveFrag_{ false };

	//死亡フラグ
	bool dieTrigger_{ false };

	//モーションのループ指定
	bool motionLoop_{ true };

	//飛んでいるか
	bool isFly_{ true };

	//無敵
	bool invincible_{ true };

	//薙ぎ払い発動したかどうか
	bool cleaverTrigger_{ false };

	//地面に触れたかどうか
	bool groundFrag_{ false };

	//ダメージ直後かどうか
	bool damageFrag_{ false };

	//ヒットエフェクト
	GSuint effectHit_{ 0 };

	//爆発エフェクト
	GSuint effectExplosion_{ 0 };

	//自身の座標
	GSvector3 myPos_{};

	//移動すべき目標地点
	GSvector3 targetPoint_{ GSvector3().zero() };

	//初めの移動用
	GSvector3 firstMovePointX_{ 50.0f, 0.0f, 0.0f };

	//ミサイルの生成座標
	GSvector3 missileMakePoint_{ GSvector3().zero() };

	//当たり判定の生成位置
	GSvector3 makeDamageRangePos_{ GSvector3().zero() };

	//ビームライフル生成位置
	GSvector3 makeBeamRiflePos_{ GSvector3().zero() };

	//ビームライフルの移動方向
	GSvector3 beamRifleVelocity_{ GSvector3().zero() };

	//プレイヤー座標
	GSvector3 playerPos_{ GSvector3().zero() };

	//マップの端
	const GSvector2 clampPosX_{ -78.0f, 195.0f };
	const GSvector2 clampPosZ_{ -11.0f, 28.0f };
};
#endif // !Boss_H_