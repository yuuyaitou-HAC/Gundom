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
	VernierState vernierstate_;

	//比較用
	VernierState ComparisonVernierstate_;

	//プレイヤーのステータスクラス
	PlayerState* playerstate_;

	//銃管理クラス
	GunControl* GC;

	ControlUnits* units_;

	mutable EnemyShip* enemyship_;

private:
	int CanBullet;

	//プレイヤーの歩く速度
	float walkSpeed{ 0.0f };

	float IsJumpTime{ 15.0f };

	//y軸回りの回転角度
	float camerayaw_{ 0.0f };

	//カメラの感度
	float CameraSensitivity;

	//EXスキル継続時間
	float EXskillTimer_ = 1800.0f;
	//EXスキル継続時間(代入)
	float assignmentExSkillTimer_ = 1800.0f;

	mutable float HPBarScale;

	//HPが一定値低下に出す煙のクールタイム
	float DastMakeTimer = 30.0f;

	float FootDastMakeTimer = 30.0f;;

	//爆破エフェクト再生時間
	float explosionTimer;

	//無敵時間
	float invincibleTimer_;
	float assignmnetInvincibleTimer_ = 120.0f;

	//ダメージ時に半透明にするための値
	float meshAlpha = 1.0f;

	bool IsJump{ false };

	bool IsAttack{ false };

	//飛んでいるか
	bool IsFly{ false };

	//EXスキル発動
	bool EXSkill_;

	//EXスキルの処理
	bool ExSkillRrocess;

	bool EXskillfinish_;

	//無敵フラグ
	bool collisionInvalid{ false };

	//自身の死亡状況
	bool DieFrag;

	//ダメージ直後かどうか
	bool damageFrag_;

	//HPが一定値以下になったら知らせるフラグ
	bool HPReductionFrag;

	//死亡時にメッシュを表示させなくするフラグ
	bool NotDrawMesh = false;

	bool test;

	//自身の座標
	GSvector3 myPos_;

	GSvector3 Dastmakepos;

	//エフェクト
private:

	//バーニア
	GSuint effectVernierL1;
	GSuint effectVernierL2;
	GSuint effectVernierS1;
	GSuint effectVernierS2;
	GSuint effectVernierSS1;
	GSuint effectVernierSS2;

	//煙
	GSuint effectDast;

	GSuint effectFootDast;

	//死亡時の爆発エフェクト
	GSuint effectExplosion;

	//ダメージ時のヒットエフェクト
	GSuint effectHit;

	//EXスキル発動時に出るエフェクト
	GSuint effectExbuff;
	GSuint effectaura;

	//テクスチャに必要な変数
private:

	//HPバー(青)
	GSvector2 HPposition{ 180,880 };
	GSrect HPRect{ 0,0,500,40 };
	GSvector2 HPScale{ 1,1 };
	GScolor4 HPColor{ 256,256,256,1.0f };

	//HPバー(灰)
	GSvector2 HPBackposition{ 680,920 };
	GSrect HPBackRect{ 0,0,500,40 };
	GScolor4 HPBackColor{ 256,256,256,1.0f };

	//必殺ゲージバー
	GSvector2 EXposition{ 180,920 };
	GSrect EXRect{ 0,0,500,20 };
	GSvector2 EXScale{ 1,1 };
	GScolor4 EXColor{ 256,256,256,1.0f };

	mutable GSvector2 enargyBarScale;

	//EXボール
	GSrect EXBallRect{ 0,0,40,40 };
	GSvector2 EXBallScale{ 1,1 };
	GScolor4 EXBallColor{ 256,256,256,1.0f };

	//EXボールの各座標
	GSvector2 EXBallposition1{ 100, 840 };
	GSvector2 EXBallposition2{ 60, 880 };
	GSvector2 EXBallposition3{ 100, 920 };

	//スラスター
	GSvector2 ThrusterBackposition{ 1210,900 };
	GSrect ThrusterBackRect{ 0,0,500,20 };
	GScolor4 ThrusterBackColor{ 256,256,256,1.0f };
	mutable GSvector2 ThrusterBackScale{ 1,1 };

	GSvector2 Thrusterposition{ 710,880 };
	GSrect ThrusterRect{ 0,0,500,20 };
	GSvector2 ThrusterScale{ 1,1 };
	GScolor4 ThrusterColor{ 256,256,256,1.0f };


	GSvector2 BeamLiflePosition{ 1300,780 };
	GSrect BeamLifleRect{ 0,0,3300,1090 };
	GSvector2 BeamLifleScale{ 0.06,0.06 };
	GSvector2 AssignmentBeamLifleScale{ 0.06,0.06 };
	GScolor4 BeamLifleColor{ 256,256,256,1.0f };

	GSvector2 BeamMagnumPosition{ 1300,850 };
	GSrect BeamMagnumRect{ 0,0,4020,2220 };
	GSvector2 BeamMagnumScale{ 0.03,0.03 };
	GSvector2 AssignmentBeamMagnumScale{ 0.03,0.03 };
	GScolor4 BeamMagnumColor{ 256,256,256,0.5f };

	GSvector2 BazookaPosition{ 1300,930 };
	GSrect BazookaRect{ 0,0,2000,660 };
	GSvector2 BazookaScale{ 0.1,0.1 };
	GSvector2 AssignmentBazookaScale{ 0.1,0.1 };
	GScolor4 BazookaColor{ 256,256,256,0.5f };

	GSvector2 BulletPosition{ 1600,780 };
	GSrect BulletRect{ 0,0,490,1135 };
	GSvector2 BulletScale{ 0.05,0.05 };
	GScolor4 BulletColor{ 256,256,256,1.0f };

	GSvector2 MagajinPosition{ 1700,780 };
	GSrect MagajinRect{ 0,0,330,500 };
	GSvector2 MagajinScale{ 0.11,0.11 };
	GScolor4 MagajinColor{ 256,256,256,1.0f };

	float magnification = 1.2f;
};
#endif