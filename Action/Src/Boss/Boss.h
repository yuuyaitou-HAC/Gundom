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
	GSuint motion_;

	//状態
	State state_;

	//ボスのステータス
	BossState* bossstate_;

	//プレイヤー
	Player* player_;

	//自陣の戦艦
	EnemyShip* enemyShip_;

private:

	//ボスの高さ
	const float bossHeight_{ 4.f };

	//衝突判定用の半径
	const float bossRadius_{ 3.5f };

	//振り返るときの速度
	const float turnAngle_{ 2.5f };

	//重力
	const float gravity_{ -0.016f };

	//足元のオフセット
	const float footOffSet_{ 0.1f };

	//受けたダメージ
	int damageValue_;

	//移動のランダム範囲
	int randPos_{ 50 };

	//爆発エフェクトの描画座標
	int randEffectPos_{ 5 };

	//ミサイル生成数
	int makeMissileNum_{ 5 };

	//状態タイマ
	float stateTimer_;

	//移動速度
	float walkSpeed_;

	//射撃クールタイム
	float beamFireCoolTime_ = 180.0f;
	float assignmentBeamFireCoolTime_ = 180.0f;

	//射撃継続時間
	float beamFireTime_ = 240.0f;
	float assignmentBeamFireTime_ = 240.0f;

	//射撃間の時間
	float fireInterval_ = 30.0f;
	float assignmentFireInterval_ = 30.0f;

	//ミサイルクールタイム
	float missileCoolTime_ = 300.0f;
	float assignmentMissileCoolTime_ = 300.0f;

	//次の爆破エフェクト再生までの時間
	float explosionTimer_ = 5.0f;
	float assignmentExplosionTimer_ = 5.0f;

	//無敵時間
	float invincibleTimer_ = 120.0f;
	float assignmentInvincibleTimer_ = 120.0f;

	//ダメージ時に半透明にするための値
	float meshAlpha_ = 1.0f;

	//ダメージ時のα値
	float damageAlpha_{ 0.5f };

	//通常時のα値
	float nomalAlpha_{ 1.0f };

	//ランダム移動先に移動し終えたかどうか
	bool randMoveFrag_;

	//死亡フラグ
	bool dieTrigger_;

	//モーションのループ指定
	bool motionLoop_;

	//飛んでいるか
	bool isFly_;

	//無敵
	bool invincible_;

	//薙ぎ払い発動したかどうか
	bool cleaverTrigger_;

	//地面に触れたかどうか
	bool groundFrag_ = false;

	//ダメージ直後かどうか
	bool damageFrag_;

	//ヒットエフェクト
	GSuint effectHit_;

	//爆発エフェクト
	GSuint effectExplosion_;

	//自身の座標
	GSvector3 myPos_;

	//移動すべき目標地点
	GSvector3 targetPoint_;

	GSvector3 targetPointX_{ 50.0f,0.0f,0.0f };

	//ミサイルの生成座標
	GSvector3 missileMakePoint_;

	//当たり判定の生成位置
	GSvector3 makeDamageRangePos_;

	//ビームライフル生成位置
	GSvector3 makeBeamRiflePos_;

	//ビームライフルの移動方向
	GSvector3 beamRifleVelocity_;

	//プレイヤー座標
	GSvector3 playerPos_;

	//マップの端
	GSvector2 clampPosX_{ -78.0f, 195.0f };
	GSvector2 clampPosZ_{ -11.0f, 28.0f };

	//輪の透明度
	float test_;

	//斬撃の半径
	float radiusTest_;
};
#endif // !Boss_H_