#ifndef COLLISION_DERECTION_H_
#define COLLISION_DERECTION_H_

#include "Actor/Actor.h"

class CollisionDerection : public Actor {

public:

	//コンストラクタ											対象のタグ			当たり判定の大きさ
	CollisionDerection(IWorld* world, const GSvector3& position, std::string target, float radius);

	virtual void draw() const override;

	//衝突判定
	virtual void react(Actor& other)override;

	//当たった相手をアクター型で返す
	Actor* actor();

	//衝突したかどうかを返す
	bool Frag();

	void setFrag(bool frag);

private:

	//フラグ
	bool frag_;

	//当たる相手
	std::string target_;

	Actor* otherActor_;

};

#endif // !COLLISION_DERECTION_H_
