#ifndef BOSS_H_
#define BOSS_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

#include "BossState.h"
#include "BossGun/BossGunController.h"


class Player;

class Boss : public Actor {

public:

	enum class State {
		Idle,		//アイドル
		Move,		//移動
		AttackMove,	//移動攻撃
		Shooting,	//射撃
		Slashing,	//斬撃
		Damage,		//ダメージ
		Die,		//死
	};

public:

	Boss(IWorld* world, const GSvector3& position);

	~Boss();

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

	BossState* bossState_()const;


private:

	//状態の更新
	void update_state(float delta_time);

	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);

	//移動
	void move(float delta_time);

	//移動攻撃
	void AttackMove(float delta_time);

	//ターゲットとの角度
	float target_signed_angle();

	//ターゲットとの距離
	float target_distance(GSvector3 Targetpos, GSvector3 pos);

	//攻撃中
	void attack(float delta_time);

	//ダメージ中
	void damage(float delta_time);

	void death(float delta_time);

	//フィールドとの衝突判定
	void collide_field();

	//アクターとの衝突判定
	void collide_actor(Actor& other);

private:
	//モーションのループ指定
	bool motion_loop_;

	//状態タイマ
	float state_timer_;

private:

	//アニメーションメッシュ
	AnimationMesh mesh_;
	//モーション番号
	GSuint motion_;

	//状態
	State state_;

	//ボスのステータスクラス
	BossState* bossstate_;

	//ボス弾管理クラス
	BossGunController* GC;

	//プレイヤー
	Player* player_;

private:

	//移動速度
	float walkSpeed{ 0.0f };

	//受けたダメージ量
	int damage_;

	//自身の座標
	GSvector3 pos;

	//アップデートで一回のみ呼び出したい処理
	bool frag;

	float ShootTime;

};

#endif // !BOSS_H_
