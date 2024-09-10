#ifndef TANK_H_
#define TANK_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

class Player;

class Tank :public Actor {

public:


	enum class State {
		Idle,		//アイドル
		Move,		//移動
		Attack,		//攻撃
		Damage,
		RunAway,	//退却
		Die			//死
	};


public:

	Tank(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	virtual void react(Actor& other)override;

private:

	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);

	void idle(float delta_time);

	void move(float delta_time);

	void attack(float delta_time);

	void damage(float delta_time);

	void runaway(float delta_time);

	void die(float delta_time);

	void generate_bullet();

private:

	//フィールドとの衝突
	void collide_field();
	//アクターとの衝突判定
	void collide_actor(Actor& other);

private:

	//アニメーションメッシュ
	AnimationMesh mesh_;
	//モーション符号
	GSuint motion_;
	//モーションのループ指定
	bool motion_loop_;
	//状態
	State state_;
	//状態タイマ
	float state_timer_;
	//プレイヤー
	//Actor* player_;
	//体力
	int health_;
	//受けたダメージ量
	int damage_;

	Player* player_;

};

#endif // !TANK_H_
