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
	AllRangeUnit::State nowstate();

	//ステータス更新
	void changestate(AllRangeUnit::State state);

	//制御から対象を受け取る
	void settarget(Actor* target);

	//現在のターゲットを返す
	Actor* retuntarget();

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

	Actor* target_;

private:

	//自身のy軸
	float posy;

	//ランダムな上下
	float randUD;

	//ランダムな左右
	float randRL;

	//プレイヤー付近でランダム座標
	bool randpos;

	bool dietrigger;

	//移動フラグ
	bool MoveFrag;

	//自身の座標
	GSvector3 pos;

	//移動量
	GSvector3 velocity_;

	//ランダムな座標
	GSvector3 RandPos;

	GSvector3 targetToVelocity_;
};

#endif // !ALL_RANGE_UNIT_H_
