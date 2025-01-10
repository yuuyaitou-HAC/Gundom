#include "HBMAI.h"
#include "Enemy/HBM.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Player/Player.h"
#include <gslib.h>
#include "BattleShip/EnemyShip.h"
#include "Collision/Ray.h"

//生成数
const int MakeNumber = 5;

//武器のランダム
GSvector2 WeaponRand = { 1,4 };

//目標地点の幅
float Range{ 10.0f };

HBMAI::HBMAI(IWorld* world, const GSvector3& position, int weapon) :
	hbms_{ MakeNumber } {

	world_ = world;

	tag_ = "EnemyAITag";

	name_ = "HBMAI";

	transform_.position(position);

	//プレイヤーの取得
	player = static_cast<Player*>(world_->find_actor("Player"));

	TargetPoint = player->transform().position();

	//銃の種類取得
	weapon_ = weapon;

	//武器ごとにプレイヤーとの差を入れる
	switch (weapon_)
	{
	case 1:
		MinDistance = 10;
		MaxDistance = 15;
		weaponangle = 180;
		break;
	case 2:
		MinDistance = 45;
		MaxDistance = 65;
		weaponangle = 45;
		break;
	case 3:
		MinDistance = 65;
		MaxDistance = 85;
		weaponangle = 30;
		break;
	case 4:
		MinDistance = 100;
		MaxDistance = 1000;
		weaponangle = 10;
		break;
	}
	//HBMの生成
	MakeHBM();
}

HBMAI::~HBMAI() {
	hbms_.clear();
}

//HBM生成
void HBMAI::MakeHBM() {

	//生成座標に自身の座標を代入
	makepos = transform_.position();

	//生成数分HBMを生成
	for (int i = 0; i < MakeNumber; i++) {
		hbms_[i] = new HBM{ world_,makepos };
		world_->add_actor(hbms_[i]);
		hbms_[i]->AttackingStrategy(weapon_);
		makepos.x += 2;
	}
}

void HBMAI::update(float delta_time) {
	//時間による制御
	MoveTimer += delta_time;

	pointtimer -= delta_time;

	Playerpos = player->transform().position();

	//目標地点
	if (weapon_ == 4) {
		//スナイパー

	}
	else {
		//その他
		MovePoint();
	}


	if (pointtimer <= 0) {
		UpdateMovePoint();
	}

	//HBMの死亡判定
	DieCheack(delta_time);
}

void HBMAI::draw() const {}

bool HBMAI::MoveTrigger() {
	//各HBMが移動中かどうか
	for (auto& hbm : hbms_) {
		if (hbm->stateNow() == 2) {

			return true;
		}
	}
	return false;
}

//スナイパー以外の目標地点渡す関数
void HBMAI::MovePoint() {

	if (MoveTimer >= 180 && !MoveTrigger()) {

		for (auto& hbm : hbms_) {

			//死亡している個体や斬撃中の個体の座標はとらない
			if (hbm->stateNow() == 7 || hbm->AttakFlag())continue;

			PlayerToHBM = GSvector3::distance(hbm->transform().position(), Playerpos);

			//一番遠いやつを入れる
			if (far < PlayerToHBM) {
				far = PlayerToHBM;
			}

			//一番近いやつを入れる
			if (close > PlayerToHBM) {
				close = PlayerToHBM;
			}
		}

		if (far > MaxDistance || close < MinDistance) {

			//斬撃
			if (weapon_ == 1) {
				SlashingMovePoint();
			}
			else {//銃撃系

			}
		}
		MoveTimer = 0;
		far = 0;
		close = 1000;
	}
}
//目標地点更新
void HBMAI::UpdateMovePoint() {

	float distance = GSvector3::distance(Playerpos, AttackMovePoint);

	if (distance >= MaxDistance || distance <= MinDistance) {

		switch (weapon_)
		{
		case 1:
			SlashingMovePoint();
			break;
		}
	}
	pointtimer = asignmentpointtimer;
}

//斬撃用の向かう目標地点
void HBMAI::SlashingMovePoint() {

	for (auto hbm : hbms_) {

		if (hbm->stateNow() == 7)continue;

		//ランダムな目標地点取得
		AttackMovePoint = SlashingRandPos();
		hbm->attackPoint(AttackMovePoint);

		//斬撃中の個体は移動状態にしない
		if (hbm->stateNow() == 4)continue;
		hbm->changeState(2);
	}
}

//斬撃用の目標地点出す関数
GSvector3 HBMAI::SlashingRandPos() {

	Ray ray = { Playerpos,-(player->transform().up()) };
	GSvector3 intersect;
	world_->field()->collide(ray, Playerpos.y + 20.0f, &intersect);

	//プレイヤーを中心にランダムな座標を求める
	GSvector3 attackpoint = GSvector3{ (float)gsRand(-MaxDistance + 1,MaxDistance - 1),0,(float)gsRand(-MaxDistance + 1,MaxDistance - 1) };
	attackpoint += Playerpos;
	attackpoint.y = intersect.y;

	if (PTRange(attackpoint)) {
		return attackpoint;
	}
	else {
		return SlashingRandPos();
	}
}

//スナイパー用の移動ポイント設定
void HBMAI::SniperMovePoint() {
	//戦艦の取得
	enemyship = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));
	GSvector3 pint = enemyship->transform().position() + transform_.forward() + centerpos;
	pint.y = 0;

	for (auto& hbm : hbms_) {

		if (hbm->stateNow() == 7)continue;

		pint.z += 4;

		hbm->attackPoint(pint);
		hbm->changeState(2);

	}
	//一回のみ呼ぶ
	SniperMovePosFlag = true;
}

void HBMAI::SniperDie() {

	//プレイヤーとHBMの最短距離を出す
	for (auto& hbm : hbms_) {

		float distance = GSvector3::distance(player->transform().position(), hbm->transform().position());

		if (distance < SniperDistence) {
			SniperDistence = distance;
		}
	}

	//HBMの最短が近さの最短より短かったら退却
	if (SniperDistence <= MinDistance) {
		//戦艦の取得
		enemyship = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));
		for (auto& hbm : hbms_) {
			if (hbm->stateNow() == 7)continue;

			GSvector3 shippos = enemyship->transform().position();
			shippos.y = 1.0f;
			GSvector3 point = shippos;

			hbm->attackPoint(point);
			hbm->changeState(6);
		}
	}
}

//部隊壊滅時の処理
void HBMAI::DieCheack(float timer) {

	for (auto& hbm : hbms_) {
		if (hbm->stateNow() == 7) {
			DieCounter++;
		}
	}

	if (weapon_ == 4) {
		SniperDie();
	}

	if (DieCounter >= 2) {
		//戦艦の取得
		enemyship = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));

		for (auto& hbm : hbms_) {
			if (hbm->stateNow() == 7)continue;

			GSvector3 shippos = enemyship->transform().position();
			Ray ray = { enemyship->transform().position(),-(transform_.up()) };
			GSvector3 intersect;
			world_->field()->collide(ray, enemyship->transform().position().y + 30.0f, &intersect);
			shippos.y = intersect.y;
			GSvector3 point = shippos;

			hbm->attackPoint(point);
			hbm->changeState(6);
		}
	}

	if (DieCounter == MakeNumber) {
		for (auto& hbm : hbms_) {
			hbm->die();
		}
		Die = true;
	}

	DieCounter = 0;
}

//ランダム座標を出して条件に合えばHBMに座標を渡す
GSvector3 HBMAI::AttackPoint() const {

	//プレイヤー近くにランダムに移動させる

	float max = MaxDistance - 1;

	float min = MaxDistance - 1;

	//ランダムで指定範囲内で座標を出す
	GSvector3 result{ gsRandf(-min,max),0.0f,gsRandf(-min,max) };

	//ランダム座標とプレイヤーの座標を足す
	result += Playerpos;

	//プレイヤーの視界内なら座標を返し視界外ならこの関数を再度呼ばせる
	if (PTRange(result)) {

		result.y = 1.f;
		return result;
	}
	return AttackPoint();
}

//自身の死を知らせる
bool HBMAI::dieTrigger() {
	return Die;
}

int HBMAI::myWeapon() {
	return weapon_;
}

//ランダム座標がプレイヤーの前方に設定されているかの判定
bool HBMAI::PTRange(GSvector3 pos) const {

	//ランダム座標とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_Target = pos - Playerpos;

	//プレイヤーの前ベクトルを求める
	GSvector3 forward = player->transform().forward();

	//各ベクトルのy要素をなくす
	forward.y = to_Target.y = 0.0f;

	//2つのベクトルのなす角度を求める
	float angle = GSvector3::signedAngle(forward, to_Target);

	float distance = GSvector3::distance(pos, Playerpos);

	//指定角度内ならtrueを返し角度外ならfalseを返す
	return (angle <= weaponangle && angle >= -weaponangle && distance >= MinDistance && MaxDistance >= distance);
}