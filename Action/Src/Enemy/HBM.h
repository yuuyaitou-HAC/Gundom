#ifndef HBM_H_
#define HBM_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

class Player;

//class HBMAI;

class HBM : public Actor {

public:

	enum class State {
		Idle,		//アイドル
		Move,		//移動
		Attack,		//攻撃
		Damage,		//ダメージ
		RunAway,	//退却
		Die			//死
	};

public:

	//コンストラクタ
	HBM(IWorld* world, const GSvector3& position);

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//判定
	virtual void react(Actor& other)override;

	//AI側からステータスの変更
	void ChangeState(int state);

	//現在のステータス
	int StateNow();

	//攻撃座標
	void AttackPoint(GSvector3 pos);

	//自身の攻撃手段
	void AttackingStrategy(int num);

private:

	//ステータスの更新
	void update_state(float delta_time);

	//ステータスの変更
	void change_state(State state, GSuint motion, bool loop = true);

	//武器描画
	void drawWeapon();

	//アイドル
	void idle(float delta_time);

	//移動
	void move(float delta_time);

	//攻撃
	void attack(float delta_time);

	//斬撃用の移動
	void SlashingMove();
	
	//斬撃
	void Slashing(float delta_time);

	//ガトリング
	void Gatring(float delta_time);

	//ビームライフル
	void BeamLifre(float delta_time);

	//スナイパー
	void Snaiper(float delta_time);

	//ダメージ
	void damage(float delta_time);

	//退却
	void runaway(float delta_time);

	//死
	void Die(float delta_time);

	//弾や当たり判定を生成
	void generate_bullet();

	//移動時の移動方向
	float target_signed_angle();

	//射撃時の向く方向
	float target_signed_angle_fire();

	//目標地点との距離
	float target_distance();

	void faceThePlayer(float delta_time);

private:

	//フィールドとの衝突
	void collide_field();
	//アクターとの衝突判定
	void collide_actor(Actor& other);

private:

	Player* player_;

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

	int health_;

	//受けたダメージ量
	int damage_;

	//目標地点
	mutable GSvector3 Destination;

	//装備中の武器
	int weapon;

	GSvector3 pos;


	bool test;
};


#endif // !HBM_H_
