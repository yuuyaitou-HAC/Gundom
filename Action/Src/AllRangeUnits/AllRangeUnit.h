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

	AllRangeUnit::State nowstate();

	void changestate(AllRangeUnit::State state);

	//制御から対象を受け取る
	void settarget(Actor* target);

private:

	//ステータス更新
	void update_state(float delta_time);

	//ステータス変更
	void change_state(State state);

	//生成
	void sortie(float delta_time);

	//攻撃
	void attack(float delta_time);

	void toPlayer(float delta_time);

	void toTarget(float delta_time);

	void generate_bullet();

	//退却
	void retreat(float delta_time);

	void deth(float delta_time);

	float target_signed_angle();

private:

	AnimationMesh mesh_;

	Player* player_;

	State state_;

	Actor* target_;

private:

	float state_timer;

	float posy;

	//ランダムな上下
	float randUD;

	//ランダムな左右
	float randRL;

	bool randpos;

	GSvector3 pos;

	GSvector3 velocity_;

};

#endif // !ALL_RANGE_UNIT_H_
