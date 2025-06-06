#ifndef PLAYER_H_
#define PLAYER_H_


#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

#include "PlayerState.h"
#include "PlayerUI.h"
#include "Gun/GunControl.h"

#include "BattleShip/EnemyShip.h"

class CameraTPS;
class ControlUnits;

//プレイヤークラス
class Player :public Actor {
public:
	//プレイヤーのステータス
	enum class State {
		Move,			//移動
		ShootAttack,	//射撃
		Damage,			//ダメージ
		Die,			//死亡
		JumpStart,		//ジャンプ開始
		Jump,			//ジャンプ中
		JumpEnd,		//着地
		MoveShootAttack,//移動中の射撃
	};

	enum class VernierState {
		up,		//上昇
		hover,	//とどまる
		down,	//下降
	};

public:
	//コンストラクタ
	Player(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.f,0.f,0.f });

	~Player();

	//更新
	void update(float delta_time)override;
	//描画
	void draw()const override;

	void draw_gui() const override;

	//衝突リアクション
	virtual void react(Actor& other)override;

public:

	PlayerState* playerState_()const;
	PlayerUI* playerUI_()const;

private:
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	//移動
	void move(float delta_time);

	//弾が０になっていないか
	bool  AttackJudgment()const;

	//状況に応じて移動攻撃か攻撃にステータスを割り振る
	void ChangeFire();

	//攻撃中
	void shoot(float delta_time);

	//攻撃中に弾が0になっていないかどうか
	void JudgementBullet();

	//ダメージ中
	void damage(float delta_time);

	void dieProcess(float delta_time);

	//ジャンプ開始
	void jump_start(float delta_time);
	//ジャンプ中
	void jump_(float delta_time);
	//着地
	void jump_end(float delta_time);

	//移動中の射撃
	void move_attack(float delta_time);

	//飛ぶ
	void Fly(float delta_time);

	//EXスキル
	void exSkill(float delta_time);

	void MakeUnit();

	//武器の描画
	void draw_weapon()const;

	//フィールドとの衝突判定
	void collide_field();
	//アクターとの衝突判定
	void collide_actor(Actor& other);
	//弾の生成
	void generate_bullet();

	//モーション中に当たり判定生成
	void can_bullet();

	void ClampPos();

	void effectUpdate(float delta_time);

private:
	//モーションのループ指定
	bool motion_loop_;

	//状態タイマ
	float state_timer_;

private:

	//アニメーションメッシュ
	AnimationMesh mesh_;
	//モーション番号
	GSuint motion_;

	//状態
	State state_;

	//飛行状態
	VernierState vernierState_;

	//比較用
	VernierState comparisonVernierstate_;

	//プレイヤーのステータスクラス
	PlayerState* playerstate_;

	PlayerUI* playerui_;

	//銃管理クラス
	GunControl* gc_;

	ControlUnits* units_;

	mutable EnemyShip* enemyShip_;

private:

	//自分の高さ
	const float playerHeight_{ 1.f };
	//衝突判定用の半径
	const float playerRadius_{ 0.5f };
	//足元のオフセット
	const float footOffset_{ 0.1f };
	//重力値
	const float gravity_{ -0.016f };

	//移動状態に応じて速度の倍率を変更
	const float groundRunSpeed_{ 1.6f };
	const float skyMoveSpeed_{ 1.3f };
	const float skyRunSpeed_{ 2.0f };

	//ジャンプ時の高さ
	const float jumpHight_{ 0.3f };


	int canBullet_;

	//プレイヤーの歩く速度
	float walkSpeed_{ 0.0f };

	float isJumpTime_{ 15.0f };

	//y軸回りの回転角度
	float cameraYaw_{ 0.0f };

	//カメラの感度
	float cameraSensitivity_;

	//EXスキル継続時間
	float exSkillTimer_ = 1800.0f;
	//EXスキル継続時間(代入)
	float assignmentExSkillTimer_ = 1800.0f;

	mutable float hpBarScale_;

	//HPが一定値低下に出す煙のクールタイム
	float dastMakeTimer_ = 30.0f;

	float footDastMakeTimer_ = 30.0f;;

	//爆破エフェクト再生時間
	float explosionTimer_;

	//無敵時間
	float invincibleTimer_ = 120.0f;
	float assignmnetInvincibleTimer_ = 120.0f;

	//ダメージ時に半透明にするための値
	float meshAlpha_ = 1.0f;

	float fireCoolDown_ = 0.0f;

	bool isJump_{ false };

	bool isAttack_{ false };

	//飛んでいるか
	bool isFly_{ false };

	//EXスキル発動
	bool exSkill_;

	//EXスキルの処理
	bool exSkillRrocess_;

	bool exSkillFinish_;

	//無敵フラグ(EXスキル用)
	bool collisionInvalid_{ false };

	//無敵フラグ(説明用)
	bool gameShowMode_{ false };

	//自身の死亡状況
	bool dieFrag_;

	//ダメージ直後かどうか
	bool damageFrag_;

	//HPが一定値以下になったら知らせるフラグ
	bool hpReductionFrag_;

	//死亡時にメッシュを表示させなくするフラグ
	bool notDrawMesh_ = false;

	bool hasFiredThisFrame_ = false;

	//自身の座標
	GSvector3 myPos_;

	GSvector3 dastMakePos_;

	GScolor exMeshColor_{ 0.8f,0.1f,0.1f,1.0f };
	GScolor nomalMeshColor_{ 0,0,0,1 };

	//エフェクト
private:

	GSmatrix4 effectWorld_;
	GSmatrix4 localMatrix_;

	//バーニア
	GSuint vernierEffectL1_;
	GSuint vernierEffectL2_;
	GSuint vernierEffectS1_;
	GSuint vernierEffectS2_;
	GSuint vernierEffectSS1_;
	GSuint vernierEffectSS2_;

	GSvector3 vernierEffectL1Pos_{ -0.1,-0.04,-0.2 };
	GSvector3 vernierEffectL2Pos_{ 0.1,-0.04,-0.2 };
	GSvector3 vernierEffectS1Pos_{ -0.1,-0.04,-0.2 };
	GSvector3 vernierEffectS2Pos_{ 0.1,-0.04,-0.2 };
	GSvector3 vernierEffectSS1Pos_{ -0.1,-0.06,-0.2 };
	GSvector3 vernierEffectSS2Pos_{ 0.1,-0.06,-0.2 };

	GSvector3 vernierEffect1Euler_{ 110.0f,30.0f,0.0f };
	GSvector3 vernierEffect2Euler_{ 110.0f,-30.0f,0.0f };

	GSvector3 vernierEffectLScale_{ 0.5,0.5,0.5 };
	GSvector3 vernierEffectSScale_{ 1,1,1 };
	GSvector3 vernierEffectSSScale_{ 1,1,1 };

	//煙
	GSuint dustEffect_;

	GScolor4 dustColor_{ 0,0,0,1 };

	GSuint footDustEffect_;

	GScolor4 footDustColor_{ 0.6,0.6, 0.6, 1 };

	//死亡時の爆発エフェクト
	GSuint explosionEffect_;

	//ダメージ時のヒットエフェクト
	GSuint hitEffect_;

	//EXスキル発動時に出るエフェクト
	GSuint exBuffEffect_;
	GSuint auraEffect_;

	GSvector3 exEffectPos_{ 0,1,0 };
	GSvector3 exEffectEuler_ = GSvector3::zero();
	GSvector3 exEffectScale_{ 1,1,1 };
};
#endif