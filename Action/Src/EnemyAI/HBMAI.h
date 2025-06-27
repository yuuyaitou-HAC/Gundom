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

	Player* player_{ NULL };

	EnemyShip* enemyShip_{ NULL };

	Weapon Weapon_{ NULL };

	//死亡した戦車の数
	int dieCounter_{ 0 };

	//中心座標妥協までの回数
	int attackPointCounter_{ 0 };

	//銃妥協までの回数
	int designatedPointCounter_{ 0 };

	//スナイパー生成数
	int makeSnuperCounter_{ 0 };

	//斬撃妥協までの回数
	int repeatCounter_{ 0 };

	//部隊生成数
	int makeNumber_{ 0 };

	//弾切れ起こした個体
	int outOfBulletCounter_{ 0 };

	//生存している個体
	int survivalCounter_{ 0 };

	//呼び出す番号
	int callNumber_{ 0 };

	//目標地点の幅
	float range_{ 10.0f };

	//移動判定時間
	float moveTimer_{ 0.0f };

	//プレイヤーと敵間の距離
	float playerDistance_{ 0.0f };

	//プレイヤーとの距離
	float minDistance_{ 0.0f };
	float maxDistance_{ 0.0f };

	//装備している武器に応じてプレイヤーの視界から広がれる角度
	float weaponAngle_{ 0.0f };

	//プレイヤーとの距離格納
	float far_{ 0 };
	const float assignmentFar_{ 0 };
	float close_{ 1000 };
	const float assignmentClose_{ 1000 };

	//目標地点とプレイヤーの座標を比較する間隔
	float pointTimer_{ 60.0f };
	const float asignmentPointTimer_{ 60.0f };

	//当たり判定の円の大きさ
	const float radius_{ 5.0f };

	//指定攻撃までの時間(斬撃、スナイパー)
	float attackTimer_{ 180.0f };

	//rayの長さ
	const float rayLength_{ 30.0f };

	//死亡フラグ
	bool die_{ false };

	//
	bool attackPointFrag_{ false };

	bool updatePoint_{ false };

	bool noPosition_;

	bool sniperMovePointTrigger_;

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

	//目標地点
	GSvector3 attackMovePoint_{ GSvector3().zero() };

	//目標地点の中心座標
	GSvector3 center_{ GSvector3().zero() };

	//プレイヤー座標のy軸を抜いたやつ
	GSvector3 playerPosXZ_{ GSvector3().zero() };

	const std::vector<float> sniperZpos_ = { 3,1,5,-1,7 };

	// それぞれのステータス
	const std::unordered_map<Weapon, GSvector3> weaponState_{
		{Weapon::BeamSaber, GSvector3{10, 15, 180}},
		{Weapon::Gatling, GSvector3{50, 60, 60}},
		{Weapon::BeamRifle, GSvector3{60, 90, 60}},
		{Weapon::Sniper, GSvector3{100, 1000, 180}},
	};
};
#endif // !HBMAI_H_