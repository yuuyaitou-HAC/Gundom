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
	void changeState(HBM::State state);

	//現在のステータス
	HBM::State stateNow();

	//攻撃座標
	void attackPoint(GSvector3 pos);

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

	// プレイヤー
	Player* player_{ NULL };

	// 爆発エフェクト
	GSuint effectExplosionL_{ 0 };

	// 被弾エフェクト
	GSuint effectHit_{ 0 };

	// バーニアエフェクト
	GSuint vernierEffect_{ 0 };

	// アニメーションメッシュ
	AnimationMesh mesh_;

	// モーション符号
	GSuint motion_;

	// 状態
	State state_{ State::Idle };

	// ダメージ前のステータス
	State frontState_{ NULL };

	// 武器の種類
	HBM::Weapon Weapon_{ NULL };

	// それぞれのステータス HP 攻撃　防御
	std::unordered_map<Weapon, GSvector3> enemyState_{
		{Weapon::BeamSaber, GSvector3{80, 32, 10}},
		{Weapon::Gatling, GSvector3{70, 30, 8}},
		{Weapon::BeamRifle, GSvector3{80, 32, 12}},
		{Weapon::Sniper, GSvector3{60, 55, 5}},
	};

	// HP
	int health_{ 0 };

	// 防御力
	int defensive_{ 0 };

	// 攻撃力
	int attackValue_{ 0 };

	// 受けたダメージ量
	int damage_{ 0 };

	// 移動方向の±
	int sign_{ 0 };

	// ガトリングの弾
	int gtringBullet_{ 20 };

	// ガトリングの弾(代入)
	int assignmentGtringBullet_{ 20 };

	// ビームライフルの弾
	int beamLifleBullet_{ 5 };

	// ビームライフルの弾(代入)
	int assignmentbeamLifleBullet_{ 5 };

	// 次の攻撃までの時間
	float attackTimer_{ 0.0f };

	// 次の移動目標地点更新までの時間
	float attackMoveTimer_{ 0.0f };

	// 斬撃攻撃をあきらめるまでの時間
	float fnishSlashTimer_{ 300.0f };

	// 斬撃攻撃放棄の基準時間
	const float fnishSlashTimeAssignment_{ 300.0f };

	// 高さ
	const float height_{ 1.f };

	// 半径
	const float radius_{ 1.f };

	// 重力
	const float gravity_{ -0.016f };

	// 弾の発射のための高さ調整
	const float footOffset_{ 0.1f };

	// 振り向き速度
	const float turnAngle_{ 2.5f };

	// 歩き速度
	const float walkSpeed_{ 0.1f };

	// ガトリング移動速度
	const float gatringWalkSpeed_{ 0.05f };

	// ビームライフル移動速度
	const float BeamLifleWalkSpeed_{ 0.07f };

	// 走るときの速度
	const float runSpeed_{ 0.4f };

	// プレイヤーのオフセット
	const float playerOffsetY_{ 1.0f };

	// 死亡時間
	const float dieTimer_{ 120.0f };

	// 状態タイマー
	float stateTimer_{ 0.0f };

	// 中心との距離
	float centerDistance_{ 0.0f };

	// エフェクト描画する距離
	const float drawEffectDistance_{ 30.0f };

	//ノックバックの強さ
	const float knockbackVelocity_{ 0.5f };

	// モーションのループ指定
	bool motionLoop_{ true };

	// 攻撃後かどうか
	bool afterSlashFrag_{ false };

	// AIに攻撃開始したか
	bool aiAttackFrag_{ false };

	// AIが攻撃後かどうか
	bool aiAfterAttackFrag_{ false };

	// 飛ぶかどうか
	bool fryTrigger_{ false };

	// 中心に向かうかどうかのフラグ
	bool moveCenterFrag_{ false };

	// メッシュを描画するかどうか
	bool drawMeshFrag_{ true };

	// 撤退のフラグ
	bool runAwayFrag_{ false };

	// 爆発エフェクト再生したかどうか
	bool playExplosionEffect_{ false };

	// 距離に応じてエフェクトを再生するかどうか
	bool playEffectDistance_{ false };

	// 今地面に接しているかどうか
	bool isGround_{ false };

	// 目標地点
	GSvector3 destination_{ GSvector3().zero() };

	// 攻撃時の移動方向
	GSvector3 attackMovePoint_{ GSvector3().zero() };

	// 自身の座標
	GSvector3 myPos_{ GSvector3().zero() };

	// プレイヤーの座標
	GSvector3 playerPos_{ GSvector3().zero() };

	// バーニアエフェクト位置
	const GSvector3 VernierEffectPos_{ 0.0f, -0.3f, -0.26f };

	// バーニアエフェクト回転
	const GSvector3 VernierEffectEuler_{ 110.0f, 0.0f, 0.0f };

	// バーニアエフェクト拡縮
	const GSvector3 VernierEffectScale_{ 0.5f, 0.5f, 0.5f };

	// 斬撃の間隔
	const GSvector2 randSlashTime_{ 60, 300 };

	// 次のランダム移動方向更新までの時間
	const GSvector2 moveRandSabel_{ 300, 600 };
	const GSvector2 moveRandGatling_{ 3, 5 };
	const GSvector2 moveRandBeamRifle_{ 2, 4 };
};
#endif // !HBM_H_