#ifndef TANK_H_
#define TANK_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

class Player;

class TankAI;

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

	//AI側からのステータスの変更
	void ChangeState(int state);

	//AI側に現在のステータスを返す
	int StateNow();

	void AttackPoint(GSvector3 pos);

private:

	//状態の更新
	void update_state(float delta_time);

	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);

	//アイドル
	void idle(float delta_time);

	//移動
	void move(float delta_time);

	//射撃
	void attack(float delta_time);

	//ダメージ
	void damage(float delta_time);

	//退散
	void runaway(float delta_time);

	//アイドル
	void Die(float delta_time);

	//弾生成
	void generate_bullet();

	//移動時の移動方向
	float target_signed_angle();

	//射撃時の向く方向
	float target_signed_angle_fire();

	//目標地点との距離
	float target_distance();

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

	//体力
	int health_;
	//受けたダメージ量
	int damage_;

	Player* player_;

	TankAI* tankAI;

	//弾発射確率
	int Fire;

	//目標地点
	mutable GSvector3 Destination;

	int DieProcessing;

};

#endif // !TANK_H_
