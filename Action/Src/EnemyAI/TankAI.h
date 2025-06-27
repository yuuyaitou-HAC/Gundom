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

	//撤退
	void retreat();

	//参照
private:

	Player* player_{ NULL };

	EnemyShip* enemyShip_{ NULL };

	//生成数
	const int MakeNumber = 5;

	//死亡した戦車の数
	int dieCounter_{ 0 };

	//プレイヤーに接近できる距離
	const int minDistance_{ 20 };
	const int maxDistance_{ 50 };

	//ランダム角度
	const int randAngle_{ 90 };

	//現在の中心座標試行回数
	int nowCenterCompromiseCount_{ 0 };
	//中心座標妥協回数
	const int centerCompromiseCount_{ 5 };

	int designatedPointCounter_{ 0 };

	//弾切れ起こした個体
	int outOfBulletCounter_{ 0 };

	//生存している個体
	int survivalCounter_{ 0 };

	//rayの長さ
	float rayLength_{ 30.0f };

	//プレイヤーとの距離格納
	float far_{ 0 };
	const float assignmentFar_{ 0 };
	float close_{ 1000 };
	const float assignmentClose_{ 1000 };

	//移動判定時間
	float moveTimer_{ 0.0f };

	//目標地点とプレイヤーの座標を比較する間隔
	float pointTimer_{ 60.0f };
	float asignmentPointTimer_{ 60.0f };

	//当たり判定の円の大きさ
	float radius_{ 5.0f };

	//プレイヤーと敵間の最小距離
	float playerToTank_{ 0.0f };

	//目標地点更新
	const float updateMovePointTime_{ 180.0f };

	//自身の死亡
	bool die_{ false };

	//目標地点設定できたか
	bool attackPointFrag_{ false };

	//撤退中か？
	bool noPosition_{ false };

	//座標更新中か
	bool updatePoint_{ false };

	bool attackFrag_{ false };

	//AIに攻撃開始したかなどを知らせるフラグ
	bool aiAttackFrag_{ false };
	bool aiAfterAttackFrag_{ false };

	bool retreatFrag_{ false };

	//マップの端
	const GSvector2 clampPosX_{ -78.0f, 195.0f };
	const GSvector2 clampPosZ_{ -11.0f, 28.0f };

	//生成場所
	GSvector3 makePos_{ GSvector3().zero() };

	//目標地点の座標
	GSvector3 playerPos_{ GSvector3().zero() };

	//タンクの座標取得
	GSvector3 tanksPos_{ GSvector3().zero() };

	//攻撃座標
	GSvector3 attackPoint_;

	//部隊の中心座標
	GSvector3 center_{ GSvector3().zero() };

	//プレイヤー座標のy軸を抜いたやつ
	GSvector3 playerPosXZ_{ GSvector3().zero() };

	//他のAIを格納する配列
	std::vector<Actor*> actors_{ NULL };
};
#endif // !TANK_AI_H_