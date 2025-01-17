#ifndef HBMAI_H_
#define HBMAI_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Enemy/HBM.h"
#include "Collision/CollisionDetection.h"
class Player;

class EnemyShip;

class HBMAI : public Actor {

private:

	//HBM管理配列
	std::vector<HBM*> hbms_;

	//ステージ上にある当たり判定をすべて格納する
	std::vector<Actor*> cds_;

public:

	HBMAI(IWorld* world, const GSvector3& position, int weapon);

	~HBMAI();

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	//死亡した隊員が一定数超えたかどうか
	bool dieTrigger();

	int myWeapon();

private:

	//戦車生成
	void MakeHBM();

	//目標地点が一定の範囲内かどうか
	bool PTRange(GSvector3 pos) const;

	//死んでいる数
	void DieCheack(float timer);

	//管轄下にある戦車が移動しているか判定
	bool MoveTrigger();

	//戦車の移動開始
	void MovePoint();

	//目標地点が条件に合わなかったときにポイントを再生成
	void UpdateMovePoint();

	//スナイパーを除く銃関係の関数
	void GunMovePoint();
	GSvector3 GunRandPos();
	GSvector3 centerOfCircle();
	GSvector3 GunAttackPoint();


	//斬撃関係の関数
	void SlashingMovePoint();
	GSvector3 SlashingRandPos();

	void retreat();

	//参照
private:

	Player* player;

	EnemyShip* enemyship;

	CollisionDerection* cd_;

	//変数
private:

	//死亡した戦車の数
	int DieCounter;

	//武器の種類
	int weapon_;

	bool Die;

	//移動判定時間
	float MoveTimer;

	//プレイヤーと敵間の最小距離
	float PlayerToHBM;

	//生成場所
	GSvector3 makepos;

	//目標地点の座標
	mutable GSvector3 Playerpos;

	//プレイヤーとの距離
	float MinDistance;
	float MaxDistance;

	//装備している武器に応じてプレイヤーの視界から広がれる角度
	float weaponangle;

	//プレイヤーとの距離が一番遠い
	float far = 0;

	//プレイヤーとの距離が一番近い
	float close = 1000;

	//目標地点とプレイヤーの座標を比較する間隔
	float pointtimer = 60.0f;
	float asignmentpointtimer = 60.0f;

	//目標地点
	GSvector3 AttackMovePoint;

	//妥協までの回数
	int attackpointcounter;

	//目標地点の中心座標
	GSvector3 center;

	bool AttackPointFrag_;

	//当たり判定の円の大きさ
	float radius = 5.0f;

	//プレイヤー座標のy軸を抜いたやつ
	GSvector3 playerposxz;

	int DesignatedPointcounter;

	bool updatepoint;

	bool noposition;

};


#endif // !HBMAI_H_