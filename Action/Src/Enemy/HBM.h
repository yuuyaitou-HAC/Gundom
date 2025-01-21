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
		Slashing,	//斬撃攻撃中
		FeintSlashing,//斬撃フェイント
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
	void changeState(int state);

	//現在のステータス
	int stateNow();

	//攻撃座標
	void attackPoint(GSvector3 pos);

	//自身の攻撃手段
	void AttackingStrategy(int num);

	bool AttakFlag();

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
	void SlashingMove(float delta_time);

	//斬撃
	void SlashingAttack(float delta_time);
	//斬撃のフェイント
	void SlashingFeint(float delta_time);

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

	int sign();

private:

	//フィールドとの衝突
	void collide_field();
	//アクターとの衝突判定
	void collide_actor(Actor& other);

private:

	Player* player_;

	//アニメーションメッシュ
	AnimationMesh mesh_;
	//モーション符号
	GSuint motion_;
	
	//状態
	State state_;

private:
	int health_;

	//受けたダメージ量
	int damage_;

	//装備中の武器
	int weapon;

	//死亡時の処理
	int DieProcessing = 0;

	int sign_;

	//状態タイマ
	float state_timer_;

	//斬撃の時間
	float AttackTimer;
	
	//攻撃時の移動時間
	float AttackMoveTimer;

	//モーションのループ指定
	bool motion_loop_;

	//近接攻撃中か
	bool SlashAttackFlag;

	bool firstUpdate;

	bool SlasingAttackFrag;

	//飛ぶかどうか
	bool frytrigger;

	//中心に向かうかどうかのフラグ
	bool BoveCenterFrag;

	//目標地点
	mutable GSvector3 Destination;

	//自身の座標
	GSvector3 pos;

	//プレイヤーの座標
	GSvector3 Playerpos;

	GSvector3 rotate_;

	//斬撃時のプレイヤーと自身の間
	GSvector3 topos;
	
	//自身の右側
	GSvector3 right;

	//斬撃の間隔
	GSvector2 RandSlashTime{ 60,300 };

	//発射の間隔
	GSvector2 AttackRandSabel{ 300,600 };
	GSvector2 AttackRandGatling{ 30,60 };
	GSvector2 AttackRandBeamRifle{ 120,180 };

};


#endif // !HBM_H_
