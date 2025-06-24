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

	//AI側からのステータスの変更
	void ChangeState(Tank::State state);

	//AI側に現在のステータスを返す
	Tank::State StateNow();

	void setattackfrag(bool frag);
	bool attackfrag()const;

	void setafterattackfrag(bool frag);
	bool afterattackfrag() const;

	void AttackPoint(GSvector3 pos);

	void SetBullet();

private:

	//状態の更新
	void update_state(float delta_time);

	//状態の変更
	void change_state(State state);

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

	//フィールドとの衝突
	void collide_field();
	//アクターとの衝突判定
	void collide_actor(Actor& other);

private:

	//アニメーションメッシュ
	AnimationMesh mesh_;

	//状態
	State state_;

	//ダメージ前のステータス
	State frontState_;

	Player* player_;

private:

	//自分の高さ
	const float EnemyHeight{ 1.f };
	//衝突判定用の半径
	const float EnemyRadius{ 1.0f };

	//重力
	const float gravity_{ -0.016f };

	const float footOffset_{ 0.1f };

	//振り向く角度
	const float turnAngle_{ 2.5f };

	//移動速度
	const float walkSpeed_{ 0.15f };

	//爆発エフェクト
	GSuint effectExplosionL_;

	//被弾エフェクト
	GSuint effectHit_;

	//体力
	int health_;

	//防御力
	int defensive_;

	//攻撃力
	int attackValue_;

	//受けたダメージ量
	int damage_;

	//弾発射確率
	int Fire;

	//戦車の弾
	int tankBullet_ = 5;

	//戦車の弾(代入)
	int assignmentTankBullet_ = 5;

	//状態タイマ
	float state_timer_;

	//次の攻撃までの時間
	float attackTime_;

	//弾生成座標のオフセットｙ
	float generateBulletOffsetY_{ 1.0f };

	//AIに攻撃開始したかなどを知らせるフラグ
	bool aiAttackFrag_;
	bool aiAfterAttackFrag_;

	//距離に応じてエフェクトを再生するかどうか
	bool playEffectDistance_;

	//撤退のフラグ
	bool runAwayFrag_;

	//爆発エフェクト再生したかどうか
	bool playExplosionEffect_;

	bool drawMeshFrag_;

	//目標地点
	GSvector3 Destination;

	//攻撃の間隔
	GSvector2 randattacktime{ 300,6000 };

	GSvector3 mypos_;
};
#endif // !TANK_H_