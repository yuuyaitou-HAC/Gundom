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

	//当たり判定
	virtual void react(Actor& other)override;

	//撤退かどうかのフラグ
	void changeFrag(bool frag);

private:

	//ユニット生成
	void makeUnits();

	//各ユニットにターゲットを渡す
	void settarget();

	//撤収命令
	void retreat();

	//各ユニットのステータス判定
	bool StateNow(AllRangeUnit::State state);

	//敵配列を管理
	void Enemyarraymanagement();

	//死亡チェック
	void diechack();

	//各ユニットにターゲットを渡す
	Actor* PickTarget();

private:

	Player* player_;

	CollisionDerection* cd_;

	std::vector<Actor*> enemys_;

private:

	//高さと幅
	const float height_{ 1.f };
	const float radius_{ 90.0f };

	int sarchCounter_;

	int dieCounter_ = 0;

	//攻撃か撤退か
	bool change_;

	GSvector3 pos_;
};
#endif // !CONTROL_UNITS_H_