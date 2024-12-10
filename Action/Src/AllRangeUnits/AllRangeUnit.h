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
		Retreat//退却
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

private:

	//ステータス更新
	void update_state(float delta_time);

	//ステータス変更
	void change_state(State state);

	//生成
	void sortie(float delta_time);

	//攻撃
	void attack(float delta_time);

	void generate_bullet();

	//退却
	void retreat(float delta_time);

private:

	AnimationMesh mesh_;

	Player* player_;

	State state_;

private:

	float state_timer;

	GSvector3 pos;

};

#endif // !ALL_RANGE_UNIT_H_
