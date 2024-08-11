#pragma once

#ifndef PLAYER_H_
#define PLAYER_H_


#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

//プレイヤークラス
class Player :public Actor {
public:
	enum class State {
		Move,
		Attack,
		Damage,
	};
public:
	//コンストラクタ
	Player(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.f,0.f,0.f });
	//更新
	void update(float delta_time)override;
	//描画
	void draw()const override;
	//衝突リアクション
	virtual void react(Actor& other)override;

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
};

#endif // !
