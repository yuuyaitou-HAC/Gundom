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
		Move,		//移動
		AttackMove,	//攻撃移動
		FryAttack,	//飛び攻撃
		Cleaver,	//薙ぎ払い
		FireBullet,	//弾発射
		Damage,		//ダメージ状態
		Die,		//死亡
	};

	//形態
	enum Form {
		first,
		second,
	};

public:

	Boss(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.f,0.f,0.f });

	~Boss();

	void update(float delta_time)override;

	void draw() const override;

	virtual void react(Actor& other)override;

	bool dieTrigger()const;

public:

	BossState* bossState_() const;

private:

	//ステータス更新
	void update_state(float delta_time);

	//ステータス変更
	void change_state(State state, GSuint motion, bool loop = true);

	//初めの移動
	void farstMove(float delta_time);

	//移動
	void move(float delta_time);

	//移動攻撃
	void attackmove(float delta_time);

	//薙ぎ払い
	void cleaver(float delta_time);

	//ダメージ
	void damage(float delta_time);

	//死亡
	void die(float delta_time);

	//射撃
	void bulletFire(float delta_time);

	//ミサイル
	void missileFire(float delta_time);

	//飛び攻撃
	void fryAttack(float delta_time);

	//対象の方向を向かせる
	void faceTheTarget(GSvector3 target, float delta_time);

	float target_signed_angle(GSvector3 target);

	//弾の生成
	void generate_bullet();

	//フィールドとの衝突判定
	void collide_field();
	//アクターとの衝突判定
	void collide_actor(Actor& other);

private:

	bool motion_loop_;

private:

	//アニメーションメッシュ
	AnimationMesh mesh_;

	//モーション番号
	GSuint motion_;

	//状態
	State state_;

	Form form_;

	//ボスのステータス
	BossState* bossstate_;

	//プレイヤー
	Player* player_;

	//自陣の戦艦
	EnemyShip* enemyship_;

private:

	//ボスの高さ
	const float BossHeight_{ 4.f };

	//衝突判定用の半径
	const float BossRadius_{ 3.5f };

	//振り返るときの速度
	const float turnAngle_{ 2.5f };

	//重力
	const float gravity_{ -0.016f };

	//足元のオフセット
	const float footOffset_{ 0.1f };

	//受けたダメージ
	int damageValue_;

	//状態タイマ
	float state_timer_;

	//移動速度
	float walkSpeed_;

	//射撃クールタイム
	float BeamFireCoolTime_ = 180.0f;
	float assignmentBeamFireCoolTime_ = 180.0f;

	//射撃継続時間
	float BeamFireTime_ = 240.0f;
	float assignmentBeamFireTime_ = 240.0f;

	//射撃間の時間
	float fireInterval_ = 30.0f;
	float assignmentFireInterval_ = 30.0f;

	//ミサイルクールタイム
	float missileCoolTime_ = 300.0f;
	float assignmentMissileCoolTime_ = 300.0f;

	//次の爆破エフェクト再生までの時間
	float exprosionTimer_ = 5.0f;
	float assignmentExprosionTimer_ = 5.0f;

	//無敵時間
	float invincibleTimer_ = 120.0f;
	float assignmnetInvincibleTimer_ = 120.0f;

	//ダメージ時に半透明にするための値
	float meshAlpha = 1.0f;

	//ランダム移動先に移動し終えたかどうか
	bool randMoveFrag_;

	//死亡フラグ
	bool dieTrigger_;

	//モーションのループ指定
	bool motion_Loop_;

	//飛んでいるか
	bool isfry_;

	//無敵
	bool invincible_;

	//薙ぎ払い発動したかどうか
	bool cleaverTrigger;

	//地面に触れたかどうか
	bool groundFrag_ = false;

	//ダメージ直後かどうか
	bool damageFrag_;

	//ヒットエフェクト
	GSuint effectHit_;

	//爆発エフェクト
	GSuint effectExprosion_;

	//自身の座標
	GSvector3 myPos_;

	//移動すべき目標地点
	GSvector3 targetPoint_;

	//ミサイルの生成座標
	GSvector3 missileMakePoint_;

	//当たり判定の生成位置
	GSvector3 makeDamageRangePos_;

	//ビームライフル生成位置
	GSvector3 makeBeamLiflePos_;

	//ビームライフルの移動方向
	GSvector3 beamLifleVelocity_;

	GSvector3 playerPos_;

	//輪の透明度
	float test_;

	//斬撃の半径
	float radiusTest_;

};

#endif // !Boss_H_
