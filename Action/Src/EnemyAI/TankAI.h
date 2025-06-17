#ifndef TANK_AI_H_
#define TANK_AI_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Enemy/Tank.h"

class Player;

class EnemyShip;

class TankAI :public Actor {

private:

	std::vector<Tank*> tanks_;

public:

	TankAI(IWorld* world, const GSvector3& position);

	~TankAI();

	virtual void update(float delta_time)override;

	virtual void draw() const override;

	bool retreatFrag()const;

	void setRetreatFrag(bool frag);

	//死亡した隊員が一定数超えたかどうか
	bool dieTrigger()const;

	//攻撃開始したか
	void setAttackFrag(bool frag);
	bool attackFrag()const;

	//攻撃後か？
	void setAfterAttackFrag(bool frag);
	bool afterAttackFrag()const;


	//関数
private:

	//戦車生成
	void MakeTank();

	//目標地点が一定の範囲内かどうか
	bool PTRange(GSvector3 pos);

	//死んでいる数
	void DieCheack(float timer);

	//管轄下にある戦車が移動しているか判定
	bool MoveTrigger();

	//戦車の移動開始
	void MovePoint();

	void Updatepoint();

	//部隊の中心座標を決める関数
	GSvector3 centerOfCircle();

	//基本の座標
	void DesignatedPoint();

	//目標地点
	virtual GSvector3 AttackPoint();

	void attack();

public:

	void retreat();


	//参照
private:

	Player* player_;

	EnemyShip* enemyShip_;

	//変数
private:

	//死亡した戦車の数
	int dieCounter_;

	//プレイヤーに接近できる距離
	int minDistance_;
	int maxDistance_;

	int attackPointCounter_;

	int designatedPointCounter_;

	//弾切れ起こした個体
	int outOfBulletCounter_;

	//生存している個体
	int survivalCounter_;

	float far_ = 0;

	float close_ = 1000;

	//死亡判定時間
	float dieTimer_;

	//移動判定時間
	float moveTimer_;

	//目標地点とプレイヤーの座標を比較する間隔
	float pointTimer_ = 60.0f;
	float asignmentPointTimer_ = 60.0f;

	//当たり判定の円の大きさ
	float radius_ = 5.0f;

	//プレイヤーと敵間の最小距離
	double playerToTank_;

	//自身の死亡
	bool dieAI_;

	//部隊の死亡状況
	bool die_;

	//目標地点設定できたか
	bool attackPointFrag_;

	//撤退中か？
	bool noPosition_;

	//座標更新中か
	bool updatePoint_;

	bool attackFrag_ = false;

	//AIに攻撃開始したかなどを知らせるフラグ
	bool aiAttackFrag_;
	bool aiAfterAttackFrag_;

	bool retreatFrag_;

	GSvector3 targetPoint_;

	//生成場所
	GSvector3 makePos_;

	//目標地点の座標
	GSvector3 playerPos_;

	//タンクの座標取得
	GSvector3 tanksPos_;

	GSvector3 result_;

	GSvector3 attackPoint_;

	//部隊の中心座標
	GSvector3 center_;

	//プレイヤー座標のy軸を抜いたやつ
	GSvector3 playerPosXZ_;

	std::vector<Actor*> actors_;
};
#endif // !TANK_AI_H_