#ifndef HBMAI_H_
#define HBMAI_H_

#include <gslib.h>
#include "Actor/Actor.h"
#include "Actor/AnimationMesh.h"
#include "Enemy/HBM.h"
#include "Common/GameData.h"

class Player;

class EnemyShip;

class HBMAI : public Actor {

private:

	//HBM管理配列
	std::vector<HBM*> hbms_;

	std::vector<Actor*> actors_;

public:

	enum class Weapon {
		Gatling,
		BeamRifle,
		BeamSaber,
		Sniper
	};

	HBMAI(IWorld* world, const GSvector3& position, HBMAI::Weapon weapon, unsigned int Generatnum);

	~HBMAI();

	virtual void update(float delta_time)override;

	virtual void draw()const override;

	//死亡した隊員が一定数超えたかどうか
	bool dieTrigger()const;

	bool retreatFrag()const;

	//自身の武器を知らせる
	HBMAI::Weapon myWeapon()const;

	//攻撃開始したか
	void setAttackFrag(bool frag);
	bool attackfrag()const;

	//攻撃後か？
	void setAfterAttackFrag(bool frag);
	bool afterAttackFrag()const;

private:

	//戦車生成
	void MakeHBM();

	//目標地点が一定の範囲内かどうか
	bool PTRange(GSvector3 pos) const;


	//死んでいる数
	void DieCheack(float timer);

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

public:

	//各個体に撤退支持を飛ばす
	void retreat();

	//参照
private:

	Player* player_;

	EnemyShip* enemyShip_;

	Weapon Weapon_;

	//変数
private:

	//死亡した戦車の数
	int dieCounter_;

	//武器の種類
	//int weapon_;

	//妥協までの回数
	int attackPointCounter_;

	int designatedPointCounter_;

	int counter_;

	//呼び出し回数
	int repeatCounter_;

	int makeNumber_;

	//弾切れ起こした個体
	int outOfBulletCounter_;

	//生存している個体
	int survivalCounter_;

	//呼び出す番号
	int callNumber_;

	//移動判定時間
	float moveTimer_;

	//プレイヤーと敵間の距離
	float playerToHBM_;

	//プレイヤーとの距離
	float minDistance_;
	float maxDistance_;

	//装備している武器に応じてプレイヤーの視界から広がれる角度
	float weaponAngle_;

	//プレイヤーとの距離格納
	float far_ = 0;
	float close_ = 1000;

	//目標地点とプレイヤーの座標を比較する間隔
	float pointTimer_ = 60.0f;
	float asignmentPointTimer_ = 60.0f;

	//当たり判定の円の大きさ
	float radius_ = 5.0f;

	//指定攻撃までの時間(斬撃、スナイパー)
	float attackTimer_ = 180.0f;

	float rayLength_{ 30.0f };

	bool die_;

	bool attackPointFrag_;

	bool updatePoint_;

	bool noPosition_;

	bool sniperMovePointTrigger_;

	bool attackFrag_ = false;

	//AIに攻撃開始したかなどを知らせるフラグ
	bool aiAttackFrag_;
	bool aiAfterAttackFrag_;

	bool retreatFrag_;

	//生成場所
	GSvector3 makePos_;

	//目標地点の座標
	mutable GSvector3 playerPos_;

	//目標地点
	GSvector3 attackMovePoint_;

	//目標地点の中心座標
	GSvector3 center_;

	//プレイヤー座標のy軸を抜いたやつ
	GSvector3 playerPosXZ_;

	std::vector<float> sniperZpos_ = { 3,1,5,-1,7 };
};


#endif // !HBMAI_H_