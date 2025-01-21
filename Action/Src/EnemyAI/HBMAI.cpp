#include "HBMAI.h"
#include "Enemy/HBM.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Player/Player.h"
#include <gslib.h>
#include "BattleShip/EnemyShip.h"
#include "Collision/Ray.h"

//目標地点の幅
float Range{ 10.0f };

HBMAI::HBMAI(IWorld* world, const GSvector3& position, int weapon,unsigned int Generatnum) :
	hbms_{ Generatnum } {

	MakeNumber = Generatnum;
	
	world_ = world;

	tag_ = "EnemyAITag";

	name_ = "HBMAI";

	transform_.position(position);

	//プレイヤーの取得
	player = static_cast<Player*>(world_->find_actor("Player"));

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
		weaponangle = 60;
		break;
	case 3:
		MinDistance = 65;
		MaxDistance = 85;
		weaponangle = 60;
		break;
	case 4:
		MinDistance = 100;
		MaxDistance = 1000;
		weaponangle = 180;
		break;
	}
	//HBMの生成
	MakeHBM();

	enemyship = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));
}

HBMAI::~HBMAI() {

	//目標地点の削除
	if (cd_ != NULL)cd_->die();
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

	playerposxz = Playerpos;
	playerposxz.y = -11.3;

	//目標地点設定
	if (weapon_ == 4) {
		if (!SniperMpvePointTrigger)SniperMovePoint();
	}
	else {
		if (!updatepoint)MovePoint();

		if (pointtimer <= 0) {
			updatepoint = true;
			UpdateMovePoint();
		}
	}

	//死亡処理
	if (weapon_ == 4)SniperDieCheack(delta_time);
	else DieCheack(delta_time);
}

void HBMAI::draw() const {}

bool HBMAI::MoveTrigger() {
	//各HBMが移動中かどうか
	for (auto& hbm : hbms_) {
		if (hbm->stateNow() == 2)return true;
	}
	return false;
}

//スナイパー以外の目標地点渡す関数
void HBMAI::MovePoint() {

	if (MoveTimer >= 180 && !MoveTrigger()) {

		for (auto& hbm : hbms_) {

			//死亡している個体や斬撃中の個体の座標はとらない
			if (hbm->stateNow() == 8 || hbm->AttakFlag())continue;

			PlayerToHBM = GSvector3::distance(hbm->transform().position(), playerposxz);

			//一番遠いやつを入れる
			if (far < PlayerToHBM)far = PlayerToHBM;

			//一番近いやつを入れる
			if (close > PlayerToHBM)close = PlayerToHBM;
		}

		if (far > MaxDistance || close < MinDistance) {

			//斬撃
			if (weapon_ == 1) {
				SlashingMovePoint();
			}
			else {//銃撃系
				AttackPointFrag_ = false;
				GunMovePoint();
			}
		}
		MoveTimer = 0;
		far = 0;
		close = 1000;
	}
}
//スナイパーの目的地
void HBMAI::SniperMovePoint() {

	for (auto& hbm : hbms_) {
		hbm->attackPoint(GSvector3{ -50,-8,SniperZpos[counter]});
		hbm->changeState(2);
		counter++;
	}
	SniperMpvePointTrigger = true;
}
//目標地点更新
void HBMAI::UpdateMovePoint() {

	float distance = GSvector3::distance(playerposxz, AttackMovePoint);

	if (distance >= MaxDistance || distance <= MinDistance) {

		if (weapon_ == 1) {
			SlashingMovePoint();
		}
		else {

			AttackPointFrag_ = false;

			GunMovePoint();

			if (noposition)retreat();
			else {
				for (auto& hbm : hbms_) {
					if (hbm->stateNow() == 8)continue;

					hbm->attackPoint(GunAttackPoint());
					hbm->changeState(2);
				}
			}

		}
	}
	pointtimer = asignmentpointtimer;
}

void HBMAI::GunMovePoint() {

	while (!AttackPointFrag_) {
		//プレイヤーに関する条件をクリアした座標を取得
		center = centerOfCircle();

		//前回の当たり判定を削除
		if (cd_ != NULL)cd_->die();

		//前回の配列を
		cds_.clear();

		//マップ内にある当たり判定全取得
		cds_ = world_->find_actor_with_tag("CollisionDerectionTag");

		//最も近い距離
		float nearDistance = 1000.0f;

		for (auto& cd : cds_) {

			//自身が生成した当たり判定を弾く
			if (cd == cd_)continue;

			float distance = GSvector3::distance(center, cd->transform().position());

			//最も近いやつを取得
			if (nearDistance > distance) {
				nearDistance = distance;
			}
		}

		//目的地に別の目的地が設定されていなかったら
		if (nearDistance > 10) {
			AttackPointFrag_ = true;
			AttackMovePoint = center;
			//ほかの部隊の目的地になっていないかを調べるための当たり判定を生成
			cd_ = new CollisionDerection{ world_,AttackMovePoint,"CollisionDerectionTag",radius };
			world_->add_actor(cd_);
			DesignatedPointcounter = 0;
		}

		DesignatedPointcounter++;
		//複数回やってもダメなら退却
		if (DesignatedPointcounter >= 5) {
			AttackPointFrag_ = true;
			noposition = true;
		}
	}
}

GSvector3 HBMAI::GunAttackPoint() {

	GSvector3 attackpoint = GSvector3{ gsRand(-radius,radius) + AttackMovePoint.x,AttackMovePoint.y,gsRand(-radius,radius) + AttackMovePoint.z };

	float distance = GSvector3::distance(AttackMovePoint, attackpoint);

	if (distance <= radius) {
		return attackpoint;
	}
	return GunAttackPoint();
}

GSvector3 HBMAI::centerOfCircle() {

	// プレイヤー近くにランダムに移動させる
	float max = MaxDistance - radius;
	float min = MinDistance + radius;

	// プレイヤーの向きを基準にランダムな角度を生成武器の角度
	float angle = gsRand(-weaponangle, weaponangle);

	// ラジアンに変換
	float radian = angle * (GS_PI / 180.0f);

	// プレイヤーの方向をベクトルとして取得
	GSvector3 playerDirection = player->transform().forward(); // プレイヤーが向いている正規化された方向ベクトル

	// 回転行列を使用して方向ベクトルを回転
	float cosTheta = cos(radian);
	float sinTheta = sin(radian);
	GSvector3 rotatedDirection{
		playerDirection.x * cosTheta - playerDirection.z * sinTheta,
		0.0f,
		playerDirection.x * sinTheta + playerDirection.z * cosTheta
	};

	// 指定距離内でランダムな位置を計算
	float distance = gsRandf(min, max);
	GSvector3 result = Playerpos + rotatedDirection * distance;

	// マップの端に抑える
	result.x = CLAMP(result.x, -78, 195);
	result.z = CLAMP(result.z, -11, 28);

	bool frag = PTRange(result);

	// プレイヤーの視界内なら座標を返し、視界外ならこの関数を再度呼び出す
	if (frag || attackpointcounter >= 5) {
		attackpointcounter = 0;

		//地面との交点を割り出した座標にする
		Ray ray = { player->transform().position(),-(transform_.up()) };
		GSvector3 intersect;
		world_->field()->collide(ray, player->transform().position().y + 20.0f, &intersect);

		result.y = intersect.y;
		return result;
	}
	attackpointcounter++;
	return centerOfCircle();
}

//斬撃用の向かう目標地点
void HBMAI::SlashingMovePoint() {

	for (auto hbm : hbms_) {

		if (hbm->stateNow() == 8)continue;

		//ランダムな目標地点取得
		AttackMovePoint = SlashingRandPos();
		hbm->attackPoint(AttackMovePoint);

		//斬撃中の個体は移動状態にしない
		if (hbm->AttakFlag())continue;
		hbm->changeState(2);
	}
}

//斬撃用の目標地点出す関数
GSvector3 HBMAI::SlashingRandPos() {

	Ray ray = { Playerpos,-(transform_.up()) };
	GSvector3 intersect;
	world_->field()->collide(ray, Playerpos.y + 20.0f, &intersect);

	//プレイヤーを中心にランダムな座標を求める
	GSvector3 attackpoint = GSvector3{ (float)gsRand(-MaxDistance + 1,MaxDistance - 1),0,(float)gsRand(-MaxDistance + 1,MaxDistance - 1) };
	attackpoint += Playerpos;
	attackpoint.y = intersect.y;

	// マップの端に抑える
	attackpoint.x = CLAMP(attackpoint.x, -78, 195);
	attackpoint.z = CLAMP(attackpoint.z, -11, 28);

	if (PTRange(attackpoint)) {
		return attackpoint;
	}
	return SlashingRandPos();
}

void HBMAI::retreat() {
	for (auto& hbm : hbms_) {
		if (hbm->stateNow() == 8)continue;

		GSvector3 shippos = enemyship->transform().position();
		Ray ray = { enemyship->transform().position(),-(transform_.up()) };
		GSvector3 intersect;
		world_->field()->collide(ray, enemyship->transform().position().y + 30.0f, &intersect);
		shippos.y = intersect.y;
		GSvector3 point = shippos;

		hbm->attackPoint(point);
		hbm->changeState(7);
	}
}

//部隊壊滅時の処理
void HBMAI::DieCheack(float timer) {

	//死亡した固体を数える
	for (auto& hbm : hbms_) {
		if (hbm->stateNow() == 8) {
			DieCounter++;
		}
	}
	//武器によって死亡処理を変える
	if (weapon_ == 4) {

	}
	else if (weapon_ == 1) {
		if (DieCounter == MakeNumber) {
			for (auto& hbm : hbms_) {
				hbm->die();
			}
			Die = true;
		}
	}
	else {
		//撤退
		if (DieCounter >= 2)retreat();
		//死亡
		if (DieCounter == MakeNumber) {
			for (auto& hbm : hbms_) {
				hbm->die();
			}
			Die = true;
		}
	}

	DieCounter = 0;
}

void HBMAI::SniperDieCheack(float timer) {

	//for (auto& hbm : hbms_) {

	//	PlayerToHBM = GSvector3::distance(Playerpos, hbm->transform().position());

	//	if (close > PlayerToHBM)close = PlayerToHBM;
	//}
	//if (close < MinDistance)retreat();
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
	GSvector3 to_Target = pos - playerposxz;

	//プレイヤーの前ベクトルを求める
	GSvector3 forward = player->transform().forward();

	//各ベクトルのy要素をなくす
	forward.y = to_Target.y = 0.0f;

	//2つのベクトルのなす角度を求める
	float angle = GSvector3::signedAngle(forward, to_Target);

	float distance = GSvector3::distance(pos, playerposxz);

	//指定角度内ならtrueを返し角度外ならfalseを返す
	return (angle <= weaponangle && angle >= -weaponangle && distance >= MinDistance && MaxDistance >= distance);
}