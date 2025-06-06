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
	AllRangeUnit::State nowstate()const;

	//ステータス更新
	void changestate(AllRangeUnit::State state);

	//制御から対象を受け取る
	void settarget(Actor* target);

	//現在のターゲットを返す
	Actor* retuntarget()const;

private:

	//ステータス更新
	void update_state(float delta_time);

	//ステータス変更
	void change_state(State state);

	//生成
	void sortie(float delta_time);

	//攻撃
	void attack(float delta_time);

	//プレイヤーに追従
	void toPlayer(float delta_time);

	//ターゲットに対し攻撃を仕掛ける
	void toTarget(float delta_time);

	//弾を生成
	void generate_bullet();

	//退却
	void retreat(float delta_time);

	//死
	void deth(float delta_time);

	GSvector3 RandPosition();

	//ターゲットとの角度を出す
	float target_signed_angle(GSvector3 target);

	int sign();

private:

	AnimationMesh mesh_;

	Player* player_;

	State state_;

	Actor* target_{ NULL };

private:

	//移動速度
	const float speed_ = 0.5f;

	//退却時の速度
	const float retrunSpeed_ = 1.5f;

	//自身のy軸
	float posY_;

	//ランダムな上下
	float randUD_;

	//ランダムな左右
	float randRL_;

	//プレイヤー付近でランダム座標
	bool randPosTrigger_;

	bool dieTrigger_;

	//移動フラグ
	bool moveFrag_;

	//自身の座標
	GSvector3 pos_;

	//ランダムな座標
	GSvector3 randPos_;

	GSvector3 targetToVelocity_;

	GSuint vernierEffect_;

	GSvector3 effectPos_{ 0.0f,0.0f,-0.1f };
	GSvector3 effectEuler_{ 0.0f,180.0f,0.0 };
	GSvector3 effectScale_{ 0.4f,0.4f,0.3f };

	GSmatrix4 world;
	GSmatrix4 local_matrix;

};

#endif // !ALL_RANGE_UNIT_H_
