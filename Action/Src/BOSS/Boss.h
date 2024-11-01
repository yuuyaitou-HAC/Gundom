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
		Slashing,	//斬撃
		Damage,		//ダメージ
		Baster,
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

	bool onTheLine(GSvector3 point);

	//ターゲットとの角度
	float target_signed_angle();

	//ターゲットとの距離
	float target_distance(GSvector3 Targetpos, GSvector3 pos);

	//攻撃中
	void shoot(float delta_time);

	void baster(float delta_time);

	//斬撃
	void slash(float delta_time);

	//後退
	void retreat();

	//ダメージ中
	void damage(float delta_time);

	void death(float delta_time);

	//プレイヤーの方向を向く
	void faceThePlayer(float delta_time);

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
	BossState* BossState_;

	//ボス弾管理クラス
	BossGunController* GC_;

	//プレイヤー
	Player* player_;

private:

	//受けたダメージ量
	int Damage;

	//次の移動までの間隔
	float Movetimer = 0.0f;

	//次の移動までの間隔(代入)
	float asignmentMoveTimer = 120.0f;

	//次の飛ぶ場所指定までの時間
	float FryTimer = 0.0f;

	//次の飛ぶ時間までの時間(代入)
	float AsignmentFryTimer = 120.0f;

	//移動速度
	float WalkSpeed{ 0.0f };

	//射撃間隔
	float ShootTime;

	//斬撃の調整前方
	float SlashDistance{ 1.5f };

	//斬撃の高さ
	float SlashHight{ 1.0f };

	//銃切り替えの距離
	float WeaponDistance;

	//飛ぶか
	bool IsFry;

	//自身の座標
	GSvector3 mypos_;

	//自身の回転
	GSvector3 rotate_;

	//プレイヤーの座標
	GSvector3 playerpos_;

	GSvector3 Point_;

	//移動攻撃のポイント
	GSvector3 Attackpoint;

	GSvector3 Frypow_;

	//飛ぶ高さのランダム
	GSvector2 FryRand{ 0,10 };
};

#endif // !BOSS_H_
