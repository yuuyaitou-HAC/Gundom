#ifndef CONTROL_UNITS_H_
#define CONTROL_UNITS_H_

#include "Actor/Actor.h"
#include "AllRangeUnit.h"
#include "Collision/CollisionDetection.h"

class Player;

class ControlUnits : public Actor {

private:
	std::vector<AllRangeUnit*> units_;

public:

	//コンストラクタ
	ControlUnits(IWorld* world, const GSvector3& position);

	//デストラクタ
	~ControlUnits();

	//更新
	virtual void update(float delta_time)override;

	//描画
	virtual void draw() const override;

	//当たり判定
	virtual void react(Actor& other)override;

	//撤退かどうかのフラグ
	void changeFrag(bool frag);

private:

	void makeUnits();

	void settarget();

	void retreat();

	bool StateNow(AllRangeUnit::State state);

	void Enemyarraymanagement();

	void diechack();

	Actor* PickTarget();

private:

	Player* player_;

	CollisionDerection* cd_;

	std::vector<Actor*> enemys_;

private:

	int sarchcounter;

	int diecounter = 0;

	//攻撃か撤退か
	bool Change;

	GSvector3 pos;
};

#endif // !CONTROL_UNITS_H_
