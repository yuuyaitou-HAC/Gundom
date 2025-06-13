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

	enum class Weapon {
		Gatling,
		BeamRifle,
		BeamSaber,
		Sniper
	};

public:

	//コンストラクタ
	HBM(IWorld* world, const GSvector3& position, HBM::Weapon Weapon_);

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
	//void AttackingStrategy(int num);

	void setattackfrag(bool frag);
	bool attackfrag()const;

	void setafterattackfrag(bool frag);
	bool afterattackfrag()const;

	//弾込め
	void SetBullet(HBM::Weapon weapon);

private:

	//距離に応じてバーニアエフェクト停止
	void vernierstop();

	//ステータスの更新
	void update_state(float delta_time);

	//ステータスの変更
	void change_state(State state, GSuint motion, bool loop = true);

	//武器描画
	void drawWeapon();

	void effectUpdate(float delta_time);

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

	float player_distance();

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

	HBM::Weapon Weapon_;

	//エフェクト
private:

	//爆発エフェクト
	GSuint effectExplosionL_;

	//被弾エフェクト
	GSuint effectHit_;

	//バーニアエフェクト
	GSuint vernierEffect_;

	GSvector3 VernierEffectPos_{ 0.0f,-0.3f,-0.26f };
	GSvector3 VernierEffectEuler_{ 110.0f,0.0f,0.0f };
	GSvector3 VernierEffectScale_{ 0.5f,0.5f,0.5f };

private:

	//高さと幅
	const float height_{ 1.f };
	const float radius_{ 1.f };

	//重力
	const float gravity_{ -0.016 };

	//弾の発射のための高さ調整
	const float footOffset_{ 0.1f };

	//振り向き速度
	const float turnAngle_{ 2.5f };

	//移動速度
	const float walkSpeed_{ 0.1f };
	const float gatringWalkSpeed_{ 0.05f };
	const float BeamLifleWalkSpeed_{ 0.07f };

	const float runSpeed_{ 0.4f };

	//HP
	int health_;

	//防御力
	int defensive_;

	//攻撃力
	int attackValue_;

	//受けたダメージ量
	int damage_;

	//装備中の武器
	//int weapon_;

	//移動方向の±
	int sign_;

	//弾数
	int gtringBulet_;
	int beamLifleBullet_;

	//次の攻撃までの時間
	float attackTimer_;

	//次の移動目標地点更新までの時間
	float attackMoveTimer_;

	//斬撃攻撃をあきらめるまでの時間
	float fnishSlashTimer_;
	const float fnishSlashTimeAssignment_ = 300.0f;

	float stateTimer_;

	//中心との距離
	float centerDistance_;

	//モーションのループ指定
	bool motionLoop_;

	//近接攻撃に向けての移動中かどうか？
	bool slashAttackMoveFlag_;

	//攻撃後かどうか
	bool afterSlashFrag_;

	//AIに攻撃開始したかなどを知らせるフラグ
	bool aiAttackFrag_;
	bool aiAfterAttackFrag_;

	//飛ぶかどうか
	bool fryTrigger_;

	//中心に向かうかどうかのフラグ
	bool moveCenterFrag_;

	//メッシュを描画するかどうか
	bool drawMeshFrag_;

	//撤退のフラグ
	bool runAwayFrag_;

	//爆発エフェクト再生したかどうか
	bool playExplosionEffect_;

	//距離に応じてエフェクトを再生するかどうか
	bool playEffectDistance_;

	//今地面に接しているかどうか
	bool isGround_;

	//ダメージ前のステータス
	State frontState_;

	//目標地点
	GSvector3 destination_;

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
	GSvector2 moveRandGatling_{ 3,5 };
	GSvector2 moveRandBeamRifle_{ 2,4 };
};
#endif // !HBM_H_