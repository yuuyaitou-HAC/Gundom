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

	enum class EXSkillState {
		None,
		Activated,   // 発動直後、効果適用タイミング
		Active,      // 効果継続中
		Finished     // 終了後のクールダウンやリセット
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

	PlayerState* player_state()const;
	PlayerUI* player_ui()const;

private:
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	//移動
	void move(float delta_time);

	//弾が０になっていないか
	bool  attack_judgment()const;

	//状況に応じて移動攻撃か攻撃にステータスを割り振る
	void change_fire();

	//攻撃中
	void shoot(float delta_time);

	//攻撃中に弾が0になっていないかどうか
	void judgement_bullet();

	//ダメージ中
	void damage(float delta_time);

	void die_process(float delta_time);

	//ジャンプ開始
	void jump_start(float delta_time);
	//ジャンプ中
	void jump(float delta_time);
	//着地
	void jump_end(float delta_time);

	//移動中の射撃
	void move_attack(float delta_time);

	//飛ぶ
	void fly(float delta_time);

	//EXスキル
	void exskill(float delta_time);

	void make_unit();

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

	//マップ内にとどめる
	void clamp_pos();

	//エフェクト更新
	void effect_update(float delta_time);
private:

	//アニメーションメッシュ
	AnimationMesh mesh_;
	//モーション番号
	GSuint motion_{ 0 };

	//状態
	State state_{ State::Move };

	EXSkillState exSkillState_{ EXSkillState::None };

	//飛行状態
	VernierState vernierState_{ VernierState::down };

	//バーニア比較用
	VernierState comparisonVernierState_{ NULL };

	//プレイヤーのステータスクラス
	PlayerState* playerstate_{ NULL };

	PlayerUI* playerui_{ NULL };

	//銃管理クラス
	GunControl* gc_{ NULL };

	ControlUnits* units_{ NULL };

private:

	//exSkillのコスト
	const int exSkillCost_[3]{ 100,200,300 };

	//状態タイマ
	float stateTimer_{ 0.0f };

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
	const float jumpHeight_{ 0.3f };

	//射撃クールタイム
	float bulletCollTimer_{ 20.0f };

	//プレイヤーの歩く速度
	float walkSpeed_{ 0.0f };

	//ジャンプクールタイム
	float jumpCollTimer_{ 15.0f };
	const float assignmentJumpCoolTimer_{ 15.0f };

	//y軸回りの回転角度
	float cameraYaw_{ 0.0f };

	//カメラの感度
	float cameraSensitivity_{ 0.8f };

	//EXスキル継続時間
	float exSkillTimer_{ 1800.0f };
	//EXスキル継続時間(代入)
	const float assignmentExSkillTimer_{ 1800.0f };

	//HPが一定値低下に出す煙のクールタイム
	float dustMakeTimer_{ 30.0f };

	//エフェクト再生タイマー
	float footDustMakeTimer_{ 30.0f };

	const float assignmentFootDustMakeTimer_{ 30.0f };

	//爆破エフェクト再生時間
	float explosionTimer_{ 180.0f };

	//無敵時間
	float invincibleTimer_{ 120.0f };
	const float assignmentInvincibleTimer_{ 120.0f };

	//ダメージ時に半透明にするための値
	float meshAlpha_{ 1.0f };

	//モーションのループ指定
	bool motionLoop_{ true };

	//ジャンプできるか
	bool isJump_{ false };

	//攻撃できるか
	bool isAttack_{ false };

	//飛んでいるか
	bool isFly_{ false };

	//無敵フラグ(EXスキル用)
	bool collisionInvalid_{ false };

	//無敵フラグ(説明用)
	bool gameShowMode_{ false };

	//自身の死亡状況
	bool dieFrag_{ false };

	//ダメージ直後かどうか
	bool damageFrag_{ false };

	//HPが一定値以下になったら知らせるフラグ
	bool hpReductionFrag_{ false };

	//死亡時にメッシュを表示させなくするフラグ
	bool notDrawMesh_{ false };

	//マップの端
	const GSvector2 clampPosX_{ -88.0f, 210.0f };
	const GSvector2 clampPosZ_{ -21.0f, 38.0f };

	//自身の座標
	GSvector3 myPos_{ GSvector3().zero() };

	//須田埃の生成座標
	GSvector3 dustMakePos_{ GSvector3().zero() };

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

	const GSvector3 vernierEffectL1Pos_{ -0.1,-0.04,-0.2 };
	const GSvector3 vernierEffectL2Pos_{ 0.1,-0.04,-0.2 };
	const GSvector3 vernierEffectS1Pos_{ -0.1,-0.04,-0.2 };
	const GSvector3 vernierEffectS2Pos_{ 0.1,-0.04,-0.2 };
	const GSvector3 vernierEffectSS1Pos_{ -0.1,-0.06,-0.2 };
	const GSvector3 vernierEffectSS2Pos_{ 0.1,-0.06,-0.2 };

	const GSvector3 vernierEffect1Euler_{ 110.0f,30.0f,0.0f };
	const GSvector3 vernierEffect2Euler_{ 110.0f,-30.0f,0.0f };

	const GSvector3 vernierEffectLScale_{ 0.5,0.5,0.5 };
	const GSvector3 vernierEffectSScale_{ 1,1,1 };
	const GSvector3 vernierEffectSSScale_{ 1,1,1 };

	//煙
	GSuint dustEffect_;

	//砂埃発生
	const GSvector3 dustRandMakePos_{ 0.5, 1, 0.5 };

	const GScolor4 dustColor_{ 0,0,0,1 };

	GSuint footDustEffect_;

	const GScolor4 footDustColor_{ 0.6,0.6, 0.6, 1 };

	//死亡時の爆発エフェクト
	GSuint explosionEffect_;

	//ダメージ時のヒットエフェクト
	GSuint hitEffect_;

	//EXスキル発動時に出るエフェクト
	GSuint exBuffEffect_;
	GSuint auraEffect_;

	const GSvector3 exEffectPos_{ 0,1,0 };
	const GSvector3 exEffectEuler_ = GSvector3::zero();
	const GSvector3 exEffectScale_{ 1,1,1 };
};
#endif