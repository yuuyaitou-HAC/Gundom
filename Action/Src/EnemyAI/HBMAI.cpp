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

HBMAI::HBMAI(IWorld* world, const GSvector3& position, int weapon, unsigned int Generatnum) :
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
		MinDistance = 50;
		MaxDistance = 60;
		weaponangle = 60;
		break;
	case 3:
		MinDistance = 60;
		MaxDistance = 90;
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

	collider_ = BoundingSphere{ Range,AttackMovePoint };

	enemyship = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));
}

HBMAI::~HBMAI() {
	actors_.clear();
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
		if (!noposition || !Die) {
			if (pointtimer <= 0) {
				updatepoint = true;
				UpdateMovePoint();
			}
			if (!updatepoint)MovePoint();
		}
	}

	//死亡処理
	if (weapon_ == 4)SniperDieCheack(delta_time);
	else DieCheack(delta_time);

	//武器がビームサーベルの時もしくはスナイパーの時に攻撃タイミングを指定する
	if (weapon_ == 1 || weapon_ == 4) {
		attack(delta_time);
	}
	else {
		GunAttack();
	}
}

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
			if (hbm->stateNow() == 8 || hbm->attackfrag())continue;

			PlayerToHBM = GSvector3::distance(hbm->transform().position(), playerposxz);

			//一番遠いやつを入れる
			if (far < PlayerToHBM)far = PlayerToHBM;

			//一番近いやつを入れる
			if (close > PlayerToHBM)close = PlayerToHBM;
		}

		if (far > MaxDistance || close < MinDistance) {

			//撤退
			if (noposition)retreat();

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
		hbm->attackPoint(GSvector3{ -50,-8,SniperZpos[counter] });
		hbm->changeState(2);
		counter++;
	}
	SniperMpvePointTrigger = true;
}
//目標地点更新
void HBMAI::UpdateMovePoint() {


	float distance = GSvector3::distance(playerposxz, AttackMovePoint);

	if (distance >= MaxDistance || distance <= MinDistance) {

		if (noposition)retreat();
		else {
			if (weapon_ == 1) SlashingMovePoint();
			else {

				AttackPointFrag_ = false;

				GunMovePoint();

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
		actors_.clear();

		//マップ内にある当たり判定全取得
		actors_ = world_->find_actor_with_tag("EnemyAITag");

		//最も近い距離
		float nearDistance = 1000.0f;

		for (auto& actor : actors_) {

			//自身が生成した当たり判定を弾く
			if (actor == this)continue;

			float distance = GSvector3::distance(center, actor->transform().position());

			//最も近いやつを取得
			if (nearDistance > distance) {
				nearDistance = distance;
			}
		}

		//目的地に別の目的地が設定されていなかったら
		if (nearDistance > 10) {
			AttackPointFrag_ = true;
			AttackMovePoint = center;
			transform_.position(AttackMovePoint);
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
		//if (hbm->attackfrag())continue;
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

	if (PTRange(attackpoint) || RepeatCounter >= 5) {
		RepeatCounter = 0;
		return attackpoint;
	}
	RepeatCounter++;
	return SlashingRandPos();
}

//スナイパーやビームサーベル部隊に命令を出す
void HBMAI::attack(float delta_time) {

	//死んでいる又はNULLの時は飛ばす
	if (hbms_[CallNumber] == NULL || hbms_[CallNumber]->stateNow() == 8) {
		CallNumber++;
		//生成数よりも呼び出しカウントが超えたらリセット
		if (CallNumber > MakeNumber - 1)CallNumber = 0;
		return;
	}

	//攻撃フラグが立っていなかったら立てる
	if (!hbms_[CallNumber]->attackfrag() && !hbms_[CallNumber]->afterattackfrag()) {
		hbms_[CallNumber]->setattackfrag(true);
	}

	//攻撃後のフラグが立っていたら指定個体の更新
	if (hbms_[CallNumber]->afterattackfrag()) {

		attacktimer -= delta_time;

		if (attacktimer <= 0) {

			//次の攻撃までの間隔
			attacktimer = 180.0f;
			//今回の呼び出した個体の攻撃後フラグを下げる
			hbms_[CallNumber]->setafterattackfrag(false);
			//呼び出す個体の更新
			CallNumber++;

			//生成数よりも呼び出しカウントが超えたらリセット
			if (CallNumber > MakeNumber - 1)CallNumber = 0;
		}
	}
}

//ライフル　ガトリング装備時に各個体に攻撃命令を出す
void HBMAI::GunAttack() {

	//管理クラスから命令が下ったかつ自身がまだ攻撃処理をしていない場合
	if (aiAttackFrag_ && !Attackfrag) {

		//攻撃処理フラグを上げる
		Attackfrag = true;

		//∀の個体に指示
		for (auto& hbm : hbms_) {
			if (hbm == NULL)continue;
			if (!hbm->attackfrag() && !getAttackFinishFrag()) {
				hbm->SetBullet(weapon_);
				hbm->setattackfrag(true);
			}
		}
	}

	//生存個体数と打ち切った個数を調べる
	for (auto& hbm : hbms_) {
		//NULLならスキップ
		if (hbm == NULL)continue;
		//弾切れしている固体をカウント
		if (hbm->afterattackfrag())outOfBulletCounter++;
		//生存している個体をカウント
		if (hbm->stateNow() != 8 && hbm->stateNow() != 7) survivalCounter++;
	}

	//生存している個体が弾を撃ち尽くしたら知らせる
	if (outOfBulletCounter == survivalCounter) {

		for (auto& hbm : hbms_) {
			//NULLならスキップ
			if (hbm == NULL)continue;
			hbm->setafterattackfrag(false);
		}
		aiAttackFrag_ = false;
		aiAfterAttackFrag_ = true;
		Attackfrag = false;
	}
	outOfBulletCounter = 0;
	survivalCounter = 0;

}

void HBMAI::setAttackFrag(bool frag) {
	aiAttackFrag_ = frag;
}

bool HBMAI::attackfrag() const {
	return aiAttackFrag_;
}

void HBMAI::setAfterAttackFrag(bool frag) {
	aiAfterAttackFrag_ = frag;
}

bool HBMAI::getAttackFinishFrag()const {
	return aiAfterAttackFrag_;
}

void HBMAI::retreat() {

	for (auto& hbm : hbms_) {
		if (hbm->stateNow() == 8)continue;

		GSvector3 shippos = enemyship->transform().position();
		Ray ray = { shippos,-(transform_.up()) };
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

	if (weapon_ == 1) {
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
bool HBMAI::dieTrigger()const {
	return Die;
}

bool HBMAI::retreatFrag() const {
	return noposition;
}

void HBMAI::setRetreatFrag(bool frag) {
	noposition = frag;
}

int HBMAI::myWeapon() const {
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