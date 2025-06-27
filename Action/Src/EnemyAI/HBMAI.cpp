#include "HBMAI.h"
#include "Enemy/HBM.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Player/Player.h"
#include <gslib.h>
#include "BattleShip/EnemyShip.h"
#include "Collision/Ray.h"

HBMAI::HBMAI(IWorld* world, const GSvector3& position, HBMAI::Weapon weapon, unsigned int Generatnum) :
	hbms_{ Generatnum } {

	makeNumber_ = Generatnum;

	world_ = world;

	tag_ = "EnemyAITag";

	name_ = "HBMAI";

	transform_.position(position);

	//プレイヤーの取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	Weapon_ = weapon;

	//武器と同じデータを見つける
	auto it = weaponState_.find(Weapon_);

	//各ステータスにデータを入れる
	if (it != weaponState_.end()) {
		minDistance_ = it->second.x;
		maxDistance_ = it->second.y;
		weaponAngle_ = it->second.z;
	}

	//HBMの生成
	MakeHBM();

	collider_ = BoundingSphere{ range_,attackMovePoint_ };

	enemyShip_ = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));
}

HBMAI::~HBMAI() {
	actors_.clear();
	hbms_.clear();
}

//HBM生成
void HBMAI::MakeHBM() {

	//生成座標に自身の座標を代入
	makePos_ = transform_.position();

	//生成数分HBMを生成
	for (int i = 0; i < makeNumber_; i++) {

		switch (Weapon_)
		{
		case HBMAI::Weapon::Gatling:
			hbms_[i] = new HBM{ world_,makePos_,HBM::Weapon::Gatling };
			break;
		case HBMAI::Weapon::BeamRifle:
			hbms_[i] = new HBM{ world_,makePos_,HBM::Weapon::BeamRifle };
			break;
		case HBMAI::Weapon::BeamSaber:
			hbms_[i] = new HBM{ world_,makePos_,HBM::Weapon::BeamSaber };
			break;
		case HBMAI::Weapon::Sniper:
			hbms_[i] = new HBM{ world_,makePos_,HBM::Weapon::Sniper };
			break;
		default:
			break;
		}

		world_->add_actor(hbms_[i]);
		makePos_.x += 2;
	}
}

void HBMAI::update(float delta_time) {
	//時間による制御
	moveTimer_ += delta_time;

	pointTimer_ -= delta_time;

	playerPos_ = player_->transform().position();

	playerPosXZ_ = playerPos_;
	Ray ray = { playerPosXZ_,-(transform_.up()) };
	GSvector3 intersect;
	world_->field()->collide(ray, player_->transform().position().y + rayLength_, &intersect);

	playerPosXZ_.y = intersect.y;

	//目標地点設定
	if (Weapon_ == HBMAI::Weapon::Sniper) {
		if (!sniperMovePointTrigger_)SniperMovePoint();
	}
	else {
		if (!noPosition_ || !die_) {
			if (pointTimer_ <= 0) {
				updatePoint_ = true;
				UpdateMovePoint();
			}
			if (!updatePoint_)MovePoint();
		}
	}

	DieCheack(delta_time);

	//武器がビームサーベルの時もしくはスナイパーの時に攻撃タイミングを指定する
	if (Weapon_ == HBMAI::Weapon::BeamSaber || Weapon_ == HBMAI::Weapon::Sniper) {
		attack(delta_time);
	}
	else {
		GunAttack();
	}
}

void HBMAI::draw() const {

	//コライダー描画フラグが立っていてかつ武器の種類がサーベルとスナイパー以外の時に描画する撤退中も表示しない
	if (world_->gameData()->drawcollider() && Weapon_ != HBMAI::Weapon::BeamSaber && Weapon_ != HBMAI::Weapon::Sniper) {
		collider().draw();
	}
}

bool HBMAI::MoveTrigger() {
	//各HBMが移動中かどうか
	for (auto& hbm : hbms_) {
		if (hbm->stateNow() == HBM::State::Move)return true;
	}
	return false;
}

//スナイパー以外の目標地点渡す関数
void HBMAI::MovePoint() {

	if (moveTimer_ >= 180 && !MoveTrigger()) {

		for (auto& hbm : hbms_) {

			//死亡している個体や斬撃中の個体の座標はとらない
			if (hbm->stateNow() == HBM::State::Die || hbm->attackfrag())continue;

			playerDistance_ = GSvector3::distance(hbm->transform().position(), playerPosXZ_);

			//一番遠いやつを入れる
			if (far_ < playerDistance_)far_ = playerDistance_;

			//一番近いやつを入れる
			if (close_ > playerDistance_)close_ = playerDistance_;
		}

		if (far_ > maxDistance_ || close_ < minDistance_) {

			//撤退
			if (noPosition_)retreat();

			//斬撃
			if (Weapon_ == HBMAI::Weapon::BeamSaber) {
				SlashingMovePoint();
			}
			else {//銃撃系
				attackPointFrag_ = false;
				GunMovePoint();
			}
		}
		moveTimer_ = 0;
		far_ = assignmentFar_;
		close_ = assignmentClose_;
	}
}
//スナイパーの目的地
void HBMAI::SniperMovePoint() {

	for (auto& hbm : hbms_) {
		hbm->attackPoint(GSvector3{ -50,-8,sniperZpos_[makeSnuperCounter_] });
		hbm->changeState(HBM::State::Move);
		makeSnuperCounter_++;
	}
	sniperMovePointTrigger_ = true;
}
//目標地点更新
void HBMAI::UpdateMovePoint() {

	float distance = GSvector3::distance(playerPosXZ_, attackMovePoint_);

	if (distance >= maxDistance_ || distance <= minDistance_) {

		if (noPosition_)retreat();
		else {
			if (Weapon_ == HBMAI::Weapon::BeamSaber) SlashingMovePoint();
			else {

				attackPointFrag_ = false;

				GunMovePoint();

				for (auto& hbm : hbms_) {
					if (hbm->stateNow() == HBM::State::Die)continue;
					hbm->attackPoint(GunAttackPoint());
					if (hbm->stateNow() != HBM::State::Move)hbm->changeState(HBM::State::Move);
				}
			}
		}
	}
	pointTimer_ = asignmentPointTimer_;
}

void HBMAI::GunMovePoint() {

	while (!attackPointFrag_) {
		//プレイヤーに関する条件をクリアした座標を取得
		center_ = centerOfCircle();
		actors_.clear();

		//マップ内にある当たり判定全取得
		actors_ = world_->find_actor_with_tag("EnemyAITag");

		//最も近い距離
		float nearDistance = 1000.0f;

		for (auto& actor : actors_) {

			//自身が生成した当たり判定を弾く
			if (actor == this)continue;

			float distance = GSvector3::distance(center_, actor->transform().position());

			//最も近いやつを取得
			if (nearDistance > distance) {
				nearDistance = distance;
			}
		}

		//目的地に別の目的地が設定されていなかったら
		if (nearDistance > 10) {
			attackPointFrag_ = true;
			attackMovePoint_ = center_;
			transform_.position(attackMovePoint_);
			designatedPointCounter_ = 0;
		}

		designatedPointCounter_++;
		//複数回やってもダメなら退却
		if (designatedPointCounter_ >= 5) {
			attackPointFrag_ = true;
			noPosition_ = true;
		}
	}
}

GSvector3 HBMAI::GunAttackPoint() {

	GSvector3 attackpoint = GSvector3{ gsRand(-radius_,radius_) + attackMovePoint_.x,attackMovePoint_.y,gsRand(-radius_,radius_) + attackMovePoint_.z };

	float distance = GSvector3::distance(attackMovePoint_, attackpoint);

	if (distance <= radius_) {
		return attackpoint;
	}
	return GunAttackPoint();
}

GSvector3 HBMAI::centerOfCircle() {

	// プレイヤー近くにランダムに移動させる
	float max = maxDistance_ - radius_;
	float min = minDistance_ + radius_;

	// プレイヤーの向きを基準にランダムな角度を生成武器の角度
	float angle = gsRand(-weaponAngle_, weaponAngle_);

	// ラジアンに変換
	float radian = angle * (GS_PI / 180.0f);

	// プレイヤーの方向をベクトルとして取得
	GSvector3 playerDirection = player_->transform().forward(); // プレイヤーが向いている正規化された方向ベクトル

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
	GSvector3 result = playerPos_ + rotatedDirection * distance;

	// マップの端に抑える
	result.x = CLAMP(result.x, clampPosX_.x, clampPosX_.y);
	result.z = CLAMP(result.z, clampPosZ_.x, clampPosZ_.y);

	bool frag = PTRange(result);

	// プレイヤーの視界内なら座標を返し、視界外ならこの関数を再度呼び出す
	if (frag || attackPointCounter_ >= 5) {
		attackPointCounter_ = 0;

		//地面との交点を割り出した座標にする
		Ray ray = { result,-(transform_.up()) };
		GSvector3 intersect;
		world_->field()->collide(ray, result.y + rayLength_, &intersect);

		result.y = intersect.y;
		return result;
	}
	attackPointCounter_++;
	return centerOfCircle();
}

//斬撃用の向かう目標地点
void HBMAI::SlashingMovePoint() {

	for (auto hbm : hbms_) {

		if (hbm->stateNow() == HBM::State::Die)continue;

		//ランダムな目標地点取得
		attackMovePoint_ = SlashingRandPos();
		hbm->attackPoint(attackMovePoint_);

		//斬撃中の個体は移動状態にしない
		//if (hbm->attackfrag())continue;
		if (hbm->stateNow() != HBM::State::Move)hbm->changeState(HBM::State::Move);
	}
}

//斬撃用の目標地点出す関数
GSvector3 HBMAI::SlashingRandPos() {

	Ray ray = { playerPos_,-(transform_.up()) };
	GSvector3 intersect;
	world_->field()->collide(ray, playerPos_.y + rayLength_, &intersect);

	//プレイヤーを中心にランダムな座標を求める
	GSvector3 attackpoint = GSvector3{ (float)gsRand(-maxDistance_ + 1,maxDistance_ - 1),0,(float)gsRand(-maxDistance_ + 1,maxDistance_ - 1) };
	attackpoint += playerPos_;
	attackpoint.y = intersect.y;

	// マップの端に抑える
	attackpoint.x = CLAMP(attackpoint.x, clampPosX_.x, clampPosX_.y);
	attackpoint.z = CLAMP(attackpoint.z, clampPosZ_.x, clampPosZ_.y);

	if (PTRange(attackpoint) || repeatCounter_ >= 5) {
		repeatCounter_ = 0;
		return attackpoint;
	}
	repeatCounter_++;
	return SlashingRandPos();
}

//スナイパーやビームサーベル部隊に命令を出す
void HBMAI::attack(float delta_time) {

	//死んでいる又はNULLの時は飛ばす
	if (hbms_[callNumber_] == NULL || hbms_[callNumber_]->stateNow() == HBM::State::Die) {
		callNumber_++;
		//生成数よりも呼び出しカウントが超えたらリセット
		if (callNumber_ > makeNumber_ - 1)callNumber_ = 0;
		return;
	}

	//攻撃フラグが立っていなかったら立てる
	if (!hbms_[callNumber_]->attackfrag() && !hbms_[callNumber_]->afterattackfrag()) {
		hbms_[callNumber_]->setattackfrag(true);
	}

	//攻撃後のフラグが立っていたら指定個体の更新
	if (hbms_[callNumber_]->afterattackfrag()) {

		attackTimer_ -= delta_time;

		if (attackTimer_ <= 0) {

			//次の攻撃までの間隔
			attackTimer_ = 180.0f;
			//今回の呼び出した個体の攻撃後フラグを下げる
			hbms_[callNumber_]->setafterattackfrag(false);
			//呼び出す個体の更新
			callNumber_++;

			//生成数よりも呼び出しカウントが超えたらリセット
			if (callNumber_ > makeNumber_ - 1)callNumber_ = 0;
		}
	}
}

//ライフル　ガトリング装備時に各個体に攻撃命令を出す
void HBMAI::GunAttack() {

	//管理クラスから命令が下ったかつ自身がまだ攻撃処理をしていない場合
	if (aiAttackFrag_ && !attackFrag_) {

		//攻撃処理フラグを上げる
		attackFrag_ = true;

		//∀の個体に指示
		for (auto& hbm : hbms_) {
			if (hbm == NULL)continue;
			if (!hbm->attackfrag() && !afterAttackFrag()) {

				if (Weapon_ == HBMAI::Weapon::Gatling) {
					hbm->SetBullet(HBM::Weapon::Gatling);
				}
				else if (Weapon_ == HBMAI::Weapon::BeamRifle) {
					hbm->SetBullet(HBM::Weapon::BeamRifle);
				}
				hbm->setattackfrag(true);
			}
		}
	}

	//生存個体数と打ち切った個数を調べる
	for (auto& hbm : hbms_) {
		//NULLならスキップ
		if (hbm == NULL)continue;
		//弾切れしている固体をカウント
		if (hbm->afterattackfrag())outOfBulletCounter_++;
		//生存している個体をカウント
		if (hbm->stateNow() != HBM::State::Die && hbm->stateNow() != HBM::State::RunAway) survivalCounter_++;
	}

	//生存している個体が弾を撃ち尽くしたら知らせる
	if (outOfBulletCounter_ == survivalCounter_) {

		for (auto& hbm : hbms_) {
			//NULLならスキップ
			if (hbm == NULL)continue;
			hbm->setafterattackfrag(false);
		}
		aiAttackFrag_ = false;
		aiAfterAttackFrag_ = true;
		attackFrag_ = false;
	}
	outOfBulletCounter_ = 0;
	survivalCounter_ = 0;
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

bool HBMAI::afterAttackFrag()const {
	return aiAfterAttackFrag_;
}

void HBMAI::retreat() {

	//撤退フラグ
	retreatFrag_ = true;

	for (auto& hbm : hbms_) {
		if (hbm->stateNow() == HBM::State::Die)continue;

		//撤退ポイントの設定
		GSvector3 shippos = enemyShip_->transform().position();
		Ray ray = { shippos,-(transform_.up()) };
		GSvector3 intersect;
		world_->field()->collide(ray, enemyShip_->transform().position().y + rayLength_, &intersect);
		shippos.y = intersect.y;
		GSvector3 point = shippos;

		//自身を撤退ポイントに持っていく
		transform_.position(point);

		//戦車に撤退座標とステータス移行させる
		hbm->attackPoint(point);
		hbm->changeState(HBM::State::RunAway);
	}
}

//部隊壊滅時の処理
void HBMAI::DieCheack(float timer) {

	//死亡した固体を数える
	for (auto& hbm : hbms_) {
		if (hbm->tag() == "DieEnemyTag") {
			dieCounter_++;
		}
	}

	if (Weapon_ == HBMAI::Weapon::BeamSaber) {
		if (dieCounter_ == makeNumber_) {
			for (auto& hbm : hbms_) {
				hbm->die();
			}
			die_ = true;
		}
	}
	else {
		//撤退
		if (dieCounter_ >= 2)retreat();
		//死亡
		if (dieCounter_ == makeNumber_) {
			for (auto& hbm : hbms_) {
				hbm->die();
			}
			die_ = true;
		}
	}
	dieCounter_ = 0;
}

//自身の死を知らせる
bool HBMAI::dieTrigger()const {
	return die_;
}

bool HBMAI::retreatFrag() const {
	return noPosition_;
}

HBMAI::Weapon HBMAI::myWeapon() const {
	return Weapon_;
}

//ランダム座標がプレイヤーの前方に設定されているかの判定
bool HBMAI::PTRange(GSvector3 pos) const {

	//ランダム座標とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_Target = pos - playerPosXZ_;

	//プレイヤーの前ベクトルを求める
	GSvector3 forward = player_->transform().forward();

	//各ベクトルのy要素をなくす
	forward.y = to_Target.y = 0.0f;

	//2つのベクトルのなす角度を求める
	float angle = GSvector3::signedAngle(forward, to_Target);

	float distance = GSvector3::distance(pos, playerPosXZ_);

	//指定角度内ならtrueを返し角度外ならfalseを返す
	return (angle <= weaponAngle_ && angle >= -weaponAngle_ && distance >= minDistance_ && maxDistance_ >= distance);
}