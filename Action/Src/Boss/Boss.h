#ifndef Boss_H_
#define Boss_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

#include "Boss/BossState.h"

class Player;

class EnemyShip;

class Boss : public Actor {

public:

	enum State {
		Move,
		AttackMove,
		FryAttack,
		Damage,
		Die,
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

	//ダメージ
	void damage(float delta_time);

	//死亡
	void die(float delta_time);

	//射撃
	void billetFire(float delta_time);

	//ミサイル
	void missileFire(float delta_time);

	//飛び攻撃
	void fryAttack(float delta_time);

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

	//ボスのステータス
	BossState* bossstate_;

	//プレイヤー
	Player* player_;

	//自陣の戦艦
	EnemyShip* enemyship_;

private:

	//受けたダメージ
	int DamageValue_;

	//状態タイマ
	float state_timer_;

	//移動速度
	float WalkSpeed_;

	//モーションのループ指定
	bool Motion_Loop_;

	//自身の座標
	GSvector3 MyPos_;

	//プレイヤーの座標
	GSvector3 PlayerPos_;


	//輪の変数
	GSvector3 scale;
	GSvector3 rotate;

	float test;

	//自身の回転
	GSvector3 Myrotate;

};

#endif // !Boss_H_
