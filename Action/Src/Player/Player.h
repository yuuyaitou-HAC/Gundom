#ifndef PLAYER_H_
#define PLAYER_H_


#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

#include "PlayerState.h"

#include "Gun/GunControl.h"

//プレイヤークラス
class Player :public Actor {
public:
	//プレイヤーのステータス
	enum class State {
		Move,			//移動
		ShootAttack,	//射撃
		SlashAttack,	//斬撃
		SecondSlash,	//二回目の斬撃
		ThirdSlash,		//三回目の斬撃
		Damage,			//ダメージ
		JumpStart,		//ジャンプ開始
		Jump,			//ジャンプ中
		JumpEnd,		//着地
		MoveShootAttack,//移動中の射撃
		MoveSlashAttack,//移動中の斬撃

	};

public:
	//コンストラクタ
	Player(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.f,0.f,0.f });

	~Player();

	//更新
	void update(float delta_time)override;
	//描画
	void draw()const override;
	//衝突リアクション
	virtual void react(Actor& other)override;

public:

	PlayerState* playerState_()const;

private:
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	//移動
	void move(float delta_time);

	//弾が０になっていないか
	void AttackJudgment();

	//攻撃にいく
	void AttackProcessing();

	void SlashProcessing();

	//攻撃中
	void attack(float delta_time);

	//一回目の斬撃
	void slash(float delta_time);

	//二回目の斬撃
	void Secondslash(float delta_time);

	//三回目の斬撃
	void Thirdslash(float delta_time);

	//攻撃中に弾が0になっていないかどうか
	void JudgementBullet();

	//ダメージ中
	void damage(float delta_time);

	//ジャンプ開始
	void jump_start(float delta_time);
	//ジャンプ中
	void jump_(float delta_time);
	//着地
	void jump_end(float delta_time);

	//移動中の射撃
	void move_attack(float delta_time);

	//移動中の斬撃
	void move_slash(float delta_time);

	//飛ぶ
	void Fly(float delta_time);

	//盾
	void Shield();

	//武器の描画
	void draw_weapon()const;

	//フィールドとの衝突判定
	void collide_field();
	//アクターとの衝突判定
	void collide_actor(Actor& other);
	//弾の生成
	void generate_bullet();

	//斬撃の生成
	void generate_attack();

	//モーション中に当たり判定生成
	void can_bullet();

	//アニメーションイベントの設定
	void SetAnimationEvent();

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

	//プレイヤーのステータスクラス
	PlayerState* playerstate_;

	//銃管理クラス
	GunControl* GC;

private:

	//プレイヤーの歩く速度
	float walkSpeed{ 0.0f };

	bool IsJump{ false };

	bool IsMoveJump{ false };
	
	float IsJumpTime{ 15.0f };

	bool IsAttack{ false };

	//飛んでいるか
	bool IsFly{ false };

	//現在のパワー(飛ぶときに使う)
	float FlyPower{ 0.0f };

	//y軸回りの回転角度
	float camerayaw_{ 0.0f };

	//攻撃手段の変更
	bool AttackChange;

	//斬撃の調整　前方
	float Distance{ 1.5f };

	//斬撃の調整　上
	float Hight{ 1.0f };

	//二回目の攻撃の判定
	bool SecondAttack_;
	
	//三回目の攻撃の判定
	bool ThirdAttack_;

	int CanBullet;

	//カメラの感度
	float CameraSensitivity;

};

#endif // !
