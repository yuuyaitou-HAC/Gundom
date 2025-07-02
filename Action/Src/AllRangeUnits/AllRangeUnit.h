#ifndef ALL_RANGE_UNIT_H_
#define ALL_RANGE_UNIT_H_

#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"

class Player;

class AllRangeUnit : public Actor {

public:

	enum class State {
		Sortie,//生成
		Attack,//攻撃
		Retreat,//退却
		Deth,
	};

public:

	//コンストラクタ
	AllRangeUnit(IWorld* world, const GSvector3& position);

	//デストラクタ
	~AllRangeUnit();

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw()const override;

	//現在のステータスを返す
	AllRangeUnit::State now_state()const;

	//ステータス更新
	void change_state(AllRangeUnit::State state);

	//制御から対象を受け取る
	void set_target(Actor* target);

	//現在のターゲットを返す
	const Actor* return_target()const;

private:

	//ステータス更新
	void update_state(float delta_time);


	//生成
	void sortie(float delta_time);

	//攻撃
	void attack(float delta_time);

	//プレイヤーに追従
	void toPlayer(float delta_time);

	//ターゲットに対し攻撃を仕掛ける
	void to_target(float delta_time);

	//弾を生成
	void generate_bullet();

	//退却
	void retreat(float delta_time);

	//死
	void deth(float delta_time);

	GSvector3 rand_position();

	//ターゲットとの角度を出す
	float target_signed_angle(GSvector3 target);

	int sign();

private:

	//メッシュ
	AnimationMesh mesh_;

	//プレイヤー
	Player* player_{ NULL };

	//ステータス
	State state_{ State::Sortie };

	//攻撃対象
	Actor* target_{ NULL };


	//移動速度
	const float speed_{ 0.5f };

	//弾の速度
	const float bulletSpeed_{ 0.5f };

	//退却時の速度
	const float returnSpeed_{ 1.5f };

	//自身のy軸
	float posY_{ 0.0f };

	//ランダムな上下
	float randUD_{ 0.0f };

	//ランダムな左右
	float randRL_{ 0.0f };

	//プレイヤーの高さ(調整)
	const float playerOffsetY_{ 3.0f };

	//ターゲットの高さ(調整)
	const float targetOffsetY_{ 1.0f };

	//ターゲットのとの距離
	const float targetDistance_{ 90.0f };

	//速度の倍率
	const float speedMagnification_{ 1.5f };

	//死亡判定する距離
	const float dieDistance_{ 2.0f };

	//ランダム座標との距離
	const float toRandPosDistance_{ 5.0f };

	//プレイヤー付近でランダム座標
	bool randPosTrigger_{ false };

	//死亡かどうか
	bool dieTrigger_{ false };

	//移動フラグ
	bool moveFlag_{ false };

	//左右のランダム範囲
	const GSvector2 randRLRange_{ -3,3 };

	//上下のランダム範囲
	const GSvector2 randUDRange_{ -3,3 };

	//ターゲットを中心に出すランダムな座標の範囲
	const GSvector2 randTargetXRange_{ -8,8 };
	const GSvector2 randTargetYRange_{ 0,8 };
	const GSvector2 randTargetZRange_{ -8,8 };

	//ランダム座標とターゲットとの距離
	const GSvector2 targetRandDistance{ 3,15 };

	//自身の座標
	GSvector3 pos_{ GSvector3().zero() };

	//プレイヤーの座標
	GSvector3 playerPos_{ GSvector3().zero() };

	//ランダムな座標
	GSvector3 randPos_{ GSvector3().zero() };

	//ターゲットの方向
	GSvector3 targetToVelocity_{ GSvector3().zero() };

	//バーニアエフェクト関係
	GSuint vernierEffect_{ 0 };
	GSvector3 effectPos_{ 0.0f,0.0f,-0.1f };
	GSvector3 effectEuler_{ 0.0f,180.0f,0.0 };
	GSvector3 effectScale_{ 0.4f,0.4f,0.3f };
	GSmatrix4 world;
	GSmatrix4 local_matrix;
};
#endif // !ALL_RANGE_UNIT_H_