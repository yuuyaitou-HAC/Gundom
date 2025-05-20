#ifndef PLAYER_H_
#define PLAYER_H_


#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

#include "PlayerState.h"
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

	//アニメーションイベントの設定
	void SetAnimationEvent();

	void ClampPos();

	void effectUpdate(float delta_time);


	//HPバーの描画
	void drawHPBer()const;

	//EXスキルバーの描画
	void drawEXBer()const;

	//武器のシルエットの描画
	void drawWeaponSilhouette()const;

	void weaponSilhouetteSize();

	//スラスター残量のber
	void drawThrusterBer()const;

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

	//銃管理クラス
	GunControl* gc_;

	ControlUnits* units_;

	mutable EnemyShip* enemyShip_;

private:
	int CanBullet;

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

	bool isJump_{ false };

	bool isAttack_{ false };

	//飛んでいるか
	bool isFly_{ false };

	//EXスキル発動
	bool exSkill_;

	//EXスキルの処理
	bool exSkillRrocess_;

	bool exSkillFinish_;

	//無敵フラグ
	bool collisionInvalid_{ false };

	//自身の死亡状況
	bool dieFrag_;

	//ダメージ直後かどうか
	bool damageFrag_;

	//HPが一定値以下になったら知らせるフラグ
	bool hpReductionFrag_;

	//死亡時にメッシュを表示させなくするフラグ
	bool notDrawMesh_ = false;

	//自身の座標
	GSvector3 myPos_;

	GSvector3 dastMakePos_;

	GScolor exMeshColor_{ 0.8f,0.1f,0.1f,1.0f };
	GScolor nomalMeshColor_{0,0,0,1};

	//レティクル
	GSrect    reticle_rect{ 0, 0, 32, 32 };
	mutable GSvector2 reticle_position;
	GSvector2 reticle_center{ 16,16 };


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

	GScolor4 footDustColor_{ 0.6,0.6, 0.6, 1};

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

	//テクスチャに必要な変数
private:

	//HPバー(青)
	GSvector2 hpBerPosition_{ 180,880 };
	GSrect hpBerRect_{ 0,0,500,40 };
	GSvector2 hpBerScale_{ 1,1 };
	GScolor4 hpBerColor_{ 256,256,256,1.0f };

	//HPバー(灰)
	GSvector2 hpBackPosition_{ 680,920 };
	GSrect hpBackRect_{ 0,0,500,40 };
	GScolor4 hpBackColor_{ 256,256,256,1.0f };

	//必殺ゲージバー
	GSvector2 exBerPosition_{ 180,920 };
	GSrect exBerRect_{ 0,0,500,20 };
	GSvector2 exBerScale_{ 1,1 };
	GScolor4 exBerColor_{ 256,256,256,1.0f };

	//EX表示
	GSvector2 exPosition_{ 115,920 };
	GSrect exRect_{ 0,0,600,300 };
	GSvector2 exScale_{ 0.07,0.07 };
	GScolor4 exColor_{ 256,256,256,1.0f };

	mutable GSvector2 enargyBarScale_;

	//EXボール
	GSrect exBallRect_{ 0,0,40,40 };
	GSvector2 exBallScale_{ 1,1 };
	GScolor4 exBallColor_{ 256,256,256,1.0f };

	//EXボールの各座標
	GSvector2 exBallPosition1_{ 100, 840 };
	GSvector2 exBallPosition2_{ 60, 880 };
	GSvector2 exBallPosition3_{ 100, 920 };

	//スラスター
	GSvector2 thrusterBackPosition_{ 1210,900 };
	GSrect thrusterBackRect_{ 0,0,500,20 };
	GScolor4 thrusterBackColor_{ 256,256,256,1.0f };
	mutable GSvector2 thrusterBackScale_{ 1,1 };

	GSvector2 thrusterPosition_{ 710,880 };
	GSrect thrusterRect_{ 0,0,500,20 };
	GSvector2 thrusterScale_{ 1,1 };
	GScolor4 thrusterColor_{ 256,256,256,1.0f };


	GSvector2 beamLiflePosition_{ 1300,780 };
	GSrect beamLifleRect_{ 0,0,3300,1090 };
	GSvector2 beamLifleScale_{ 0.06,0.06 };
	GSvector2 assignmentBeamLifleScale_{ 0.06,0.06 };
	GScolor4 beamLifleColor_{ 256,256,256,1.0f };

	GSvector2 beamMagnumPosition_{ 1300,850 };
	GSrect beamMagnumRect_{ 0,0,4020,2220 };
	GSvector2 beamMagnumScale_{ 0.03,0.03 };
	GSvector2 assignmentBeamMagnumScale_{ 0.03,0.03 };
	GScolor4 beamMagnumColor_{ 256,256,256,0.5f };

	GSvector2 bazookaPosition_{ 1300,930 };
	GSrect bazookaRect_{ 0,0,2000,660 };
	GSvector2 bazookaScale_{ 0.1,0.1 };
	GSvector2 assignmentBazookaScale_{ 0.1,0.1 };
	GScolor4 bazookaColor_{ 256,256,256,0.5f };

	GSvector2 bulletPosition_{ 1600,780 };
	GSrect bulletRect_{ 0,0,490,1135 };
	GSvector2 bulletScale_{ 0.05,0.05 };
	GScolor4 bulletColor_{ 256,256,256,1.0f };

	GSvector2 magajinPosition_{ 1700,780 };
	GSrect magajinRect_{ 0,0,330,500 };
	GSvector2 magajinScale_{ 0.11,0.11 };
	GScolor4 magajinColor_{ 256,256,256,1.0f };

	GSvector2 hpPosition_{ 115,880 };
	GSrect hpRect_{ 0,0,600,300 };
	GSvector2 hpScale_{ 0.1,0.1 };
	GScolor4 hpColor_{ 256,256,256,1.0f };

	//数値
	mutable GSvector2 numPos_;
	GSvector2 numScale_{ 0.5,0.5 };
	GSrect numRect_[10]{
	GSrect{0,  0, 60,  60},
	GSrect{0, 60, 60, 120},
	GSrect{0,120, 60, 180},
	GSrect{0,180, 60, 240},
	GSrect{0,240, 60, 300},
	GSrect{0,300, 60, 360},
	GSrect{0,360, 60, 420},
	GSrect{0,420, 60, 480},
	GSrect{0,480, 60, 540},
	GSrect{0,540, 60, 600}
	};
	GScolor4 numColor_{ 256,256,256,1.0f };

	//＊
	mutable GSvector2 asteriskPosition_;
	GSrect asteriskRect_{ 0,0,60,60 };
	GSvector2 asteriskScale_{ 0.5,0.5 };
	GScolor4 asteriskColor_{ 256,256,256,1.0f };

	mutable GSrect bulletNum_;

	float magnification_ = 1.2f;
};
#endif