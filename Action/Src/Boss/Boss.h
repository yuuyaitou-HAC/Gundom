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

	void draw_gui() const override;

	virtual void react(Actor& other)override;

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

	//受けたダメージ
	int damageValue_;

	//状態タイマ
	float state_timer_;

	//移動速度
	float walkSpeed_;

	//射撃クールタイム
	float fireCoolTime_ = 180.0f;
	float assignmentFireCoolTime_ = 180.0f;

	//射撃継続時間
	float fireTime_ = 240.0f;
	float assignmentFireTime_ = 240.0f;

	float fireInterval_ = 30.0f;
	float assignmentFireInterval_ = 30.0f;

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

	//ヒットエフェクト
	GSuint effectHit_;

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
