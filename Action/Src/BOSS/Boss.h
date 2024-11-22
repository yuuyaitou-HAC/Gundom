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

	//コンストラクタ
	Boss(IWorld* world, const GSvector3& position);

	//デストラクタ
	~Boss();

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//衝突判定
	virtual void react(Actor& other)override;

	//ボスステータス
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

	void inertia(float delta_time);

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

	//ボスのステータスクラス
	BossState* bossstate_;

	//ボス弾管理クラス
	BossGunController* GC_;

	//プレイヤー
	Player* player_;

private:

	//受けたダメージ量
	int Damage_;

	//状態タイマ
	float Atate_Timer_;

	//次の移動までの間隔
	float MoveTimer_ = 0.0f;

	//次の移動までの間隔(代入)
	float AsignmentMoveTimer_ = 120.0f;

	//次の飛ぶ場所指定までの時間
	float FryTimer_ = 0.0f;

	//次の飛ぶ時間までの時間(代入)
	float AsignmentFryTimer_ = 120.0f;

	//移動速度
	float WalkSpeed_{ 0.0f };

	//射撃間隔
	float ShootTime_;

	//斬撃の調整前方
	float SlashDistance_{ 1.5f };

	//斬撃の高さ
	float SlashHight_{ 1.0f };

	//銃切り替えの距離
	float WeaponDistance_;

	//バスターライフル発射までの時間
	//仮置き　モーションができたらモーションの時間でやる
	float BasterTimer_ = 120.0f;

	//モーションのループ指定
	bool Motion_Loop_;

	//飛ぶか
	bool IsFry_;

	//自身の座標
	GSvector3 MyPos_;

	//自身の回転
	GSvector3 Rotate_;

	//プレイヤーの座標
	GSvector3 PlayerPos_;

	GSvector3 Point_;

	//移動攻撃のポイント
	GSvector3 Attackpoint_;

	GSvector3 Frypow_;

	//飛ぶ高さのランダム
	GSvector2 FryRand_{ 0,10 };

	GSvector3 direction;

	GSvector3 MoveTo_;

};

#endif // !BOSS_H_
