#ifndef HBM_H_
#define HBM_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

class Player;

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

	void setattackfrag(bool frag);
	bool attackfrag()const;

	void setafterattackfrag(bool frag);
	bool afterattackfrag()const;

	//弾込め
	void SetBullet(int weapon);

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
	int weapon_;

	//移動方向の±
	int sign_;

	//弾数
	int gtringBulet_;
	int beamLifleBullet_;

	//プレイヤーとの距離
	float playerDistance_;

	//次の攻撃までの時間
	float attackTimer_;

	//次の移動目標地点更新までの時間
	float attackMoveTimer_;

	//斬撃攻撃をあきらめるまでの時間
	float fnishSlashTimer_;
	float fnishSlashTimeAssignment_ = 300.0f;

	//モーションのループ指定
	bool motion_loop_;

	//近接攻撃に向けての移動中かどうか？
	bool slashAttackMoveFlag_;

	//攻撃後かどうか
	bool afterSlashFrag_;

	//AIに攻撃開始したかなどを知らせるフラグ
	bool aiAttackFrag_;
	bool aiAfterAttackFrag_;

	//飛ぶかどうか
	bool frytrigger_;

	//中心に向かうかどうかのフラグ
	bool moveCenterFrag_;

	//目標地点
	GSvector3 destination;

	//攻撃時の移動方向
	GSvector3 attackMovePoint_;

	//自身の座標
	GSvector3 myPos_;

	//プレイヤーの座標
	GSvector3 playerPos_;

	//斬撃の間隔
	GSvector2 randSlashTime_{ 60,300 };

	//次のランダム移動方向更新までの時間
	GSvector2 moveRandSabel_{ 300,600 };
	GSvector2 moveRandGatling_{ 30,60 };
	GSvector2 moveRandBeamRifle_{ 120,180 };
};
#endif // !HBM_H_