#ifndef HBMAI_H_
#define HBMAI_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Enemy/HBM.h"

class Player;

class EnemyShip;

class HBMAI : public Actor {

private:

	//HBM管理配列
	std::vector<HBM*> hbms_;

	std::vector<Actor*> actors_;

public:

	HBMAI(IWorld* world, const GSvector3& position, int weapon, unsigned int Generatnum);

	~HBMAI();

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	//死亡した隊員が一定数超えたかどうか
	bool dieTrigger();

	//自身が撤退中かどうかを知らせる
	bool RetrunRetreatFrag();

	//自身の武器を知らせる
	int myWeapon();

	//攻撃開始したか
	void setattackfrag(bool frag);
	bool attackfrag();

	//攻撃後か？
	void setafterattackfrag(bool frag);
	bool afterattackfrag();

private:

	//戦車生成
	void MakeHBM();

	//目標地点が一定の範囲内かどうか
	bool PTRange(GSvector3 pos) const;


	//死んでいる数
	void DieCheack(float timer);
	//スナイパー用
	void SniperDieCheack(float timer);

	//管轄下にある戦車が移動しているか判定
	bool MoveTrigger();

	//HBMの移動開始
	void MovePoint();

	//スナイパー部隊の移動ポイント指示
	void SniperMovePoint();

	//目標地点が条件に合わなかったときにポイントを再生成
	void UpdateMovePoint();

	//スナイパーを除く銃関係の関数
	void GunMovePoint();
	GSvector3 centerOfCircle();
	GSvector3 GunAttackPoint();


	//斬撃関係の関数
	void SlashingMovePoint();
	GSvector3 SlashingRandPos();

	//スナイパー斬撃用の攻撃命令関数
	void attack(float delta_time);

	//ライフルガトリング用の攻撃命令関数
	void GunAttack();

	//各個体に撤退支持を飛ばす
	void retreat();

	//参照
private:

	Player* player;

	EnemyShip* enemyship;

	//変数
private:

	//死亡した戦車の数
	int DieCounter;

	//武器の種類
	int weapon_;

	//妥協までの回数
	int attackpointcounter;

	int DesignatedPointcounter;

	int counter;

	//呼び出し回数
	int RepeatCounter;

	int MakeNumber;

	//弾切れ起こした個体
	int outOfBulletCounter;

	//生存している個体
	int survivalCounter;

	//呼び出す番号
	int CallNumber;

	//移動判定時間
	float MoveTimer;

	//プレイヤーと敵間の距離
	float PlayerToHBM;

	//プレイヤーとの距離
	float MinDistance;
	float MaxDistance;

	//装備している武器に応じてプレイヤーの視界から広がれる角度
	float weaponangle;

	//プレイヤーとの距離格納
	float far = 0;
	float close = 1000;

	//目標地点とプレイヤーの座標を比較する間隔
	float pointtimer = 60.0f;
	float asignmentpointtimer = 60.0f;

	//当たり判定の円の大きさ
	float radius = 5.0f;

	//指定攻撃までの時間(斬撃、スナイパー)
	float attacktimer = 180.0f;

	bool Die;

	bool AttackPointFrag_;

	bool updatepoint;

	bool noposition;

	bool SniperMpvePointTrigger;

	//撤退フラグ
	bool retreatFrag;

	bool Attackfrag = false;

	//AIに攻撃開始したかなどを知らせるフラグ
	bool AIAttackFrag;
	bool AIAfterAttackFrag;

	//生成場所
	GSvector3 makepos;

	//目標地点の座標
	mutable GSvector3 Playerpos;

	//目標地点
	GSvector3 AttackMovePoint;

	//目標地点の中心座標
	GSvector3 center;

	//プレイヤー座標のy軸を抜いたやつ
	GSvector3 playerposxz;

	std::vector<float> SniperZpos = { 3,1,5,-1,7 };
};


#endif // !HBMAI_H_