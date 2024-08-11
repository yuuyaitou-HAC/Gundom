#ifndef PLAYER_H_
#define PLAYER_H_


#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

#include "PlayerState.h"

//プレイヤークラス
class Player :public Actor {
public:
	enum class State {
		Move,		//移動
		Attack,		//攻撃
		Damage,		//ダメージ
		JumpStart,  //ジャンプ開始
		Jump,    //ジャンプ中
		JumpEnd,    //着地
		MoveAttack,//移動中の攻撃
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
	//攻撃中
	void attack(float delta_time);
	//ダメージ中
	void damage(float delta_time);

	//ジャンプ開始
	void jump_start(float delta_time);
	//ジャンプ中
	void jump_(float delta_time);
	//着地
	void jump_end(float delta_time);

	//移動攻撃
	void move_attack(float delta_time);

	void Fly(float delta_time);

	//武器の描画
	void draw_weapon()const;

	//フィールドとの衝突判定
	void collide_field();
	//アクターとの衝突判定
	void collide_actor(Actor& other);
	//弾の生成
	void generate_bullet();

private:
	//アニメーションメッシュ
	AnimationMesh mesh_;
	//モーション番号
	GSuint motion_;
	//モーションのループ指定
	bool motion_loop_;
	//状態
	State state_;
	//状態タイマ
	float state_timer_;

	PlayerState* playerstate_;

private:

	float walkSpeed{ 0.0f };

	bool IsJump{ false };
	bool IsMoveJump{ false };
	float IsJumpTime{ 15.0f };

	bool IsAttack{ false };

	//飛んでいるか
	bool IsFly{ false };

	//現在のパワー(飛ぶときに使う)
	float FryPower{ 0.0f };


};

#endif // !
