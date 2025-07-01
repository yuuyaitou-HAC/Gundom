#include "TankAI.h"
#include <gslib.h>
#include "Enemy/Tank.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Player/Player.h"
#include "BattleShip/EnemyShip.h"
#include "Collision/Ray.h"

//生成数
const int MakeNumber = 5;

TankAI::TankAI(IWorld* world, const GSvector3& position) :
	tanks_(MakeNumber),
	retreatFrag_{ false } {

	world_ = world;

	tag_ = "EnemyAITag";

	name_ = "TankAI";

	transform_.position(position);

	//プレイヤー取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));

	//自身の戦艦を取得
	enemyShip_ = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));

	collider_ = BoundingSphere{ radius_,attackPoint_ };

	//戦車の生成
	MakeTank();
}

TankAI::~TankAI() {
	tanks_.clear();
	actors_.clear();
}

//戦車生成
void TankAI::MakeTank() {

	//生成座標に自身の座標を代入
	makePos_ = transform_.position();

	//生成数分戦車を生成
	for (int i = 0; i < MakeNumber; i++) {
		tanks_[i] = new Tank{ world_,makePos_ };
		world_->add_actor(tanks_[i]);
		makePos_.x += 2;
	}
}

void TankAI::update(float delta_time) {

	//時間による制御
	moveTimer_ += delta_time;

	pointTimer_ -= delta_time;

	playerPos_ = player_->transform().position();

	playerPosXZ_ = playerPos_;

	Ray ray = { playerPosXZ_,-(transform_.up()) };
	GSvector3 intersect;
	world_->field()->collide(ray, player_->transform().position().y + rayLength_, &intersect);

	playerPosXZ_.y = intersect.y;

	if (!noPosition_ || !die_) {

		if (pointTimer_ <= 0) {
			updatePoint_ = true;
			Updatepoint();
		}

		if (!updatePoint_)MovePoint();
	}

	//戦車の死亡判定
	DieCheack(delta_time);

	//各個体に命令を出す
	attack();
}

void TankAI::draw() const {
	if (world_->gameData()->drawcollider()) {
		collider().draw();
	}
}

bool TankAI::MoveTrigger() {
	//各戦車が移動中かどうか
	for (auto& tank : tanks_) {

		if (tank->StateNow() == Tank::State::Move) {
			return true;
		}
	}
	return false;
}

void TankAI::MovePoint() {

	//一定時間経過かつ移動中フラグがなければ
	if (moveTimer_ >= updateMovePointTime_ && !MoveTrigger()) {

		for (auto& tank : tanks_) {

			//死亡している個体はスキップ
			if (tank->StateNow() == Tank::State::Die)continue;

			//タンク座標取得
			tanksPos_ = tank->transform().position();

			//プレイヤーとタンクの距離を取る
			playerToTank_ = GSvector3::distance(playerPosXZ_, tanksPos_);

			if (far_ < playerToTank_)far_ = playerToTank_;
			if (close_ > playerToTank_)close_ = playerToTank_;

		}
		//距離が一定以内なら移動開始
		if (far_ > maxDistance_ || close_ < minDistance_) {

			attackPointFrag_ = false;

			//当たり判定二生成と部隊の移動すべき座標を取得
			DesignatedPoint();

			if (noPosition_)retreat();
			else {
				for (auto& tank : tanks_) {

					//死亡している個体はスキップ
					if (tank->StateNow() == Tank::State::Die)continue;

					tank->AttackPoint(AttackPoint());
					tank->ChangeState(Tank::State::Move);
				}
			}
		}
		moveTimer_ = 0;
		far_ = assignmentFar_;
		close_ = assignmentClose_;
	}
}

//定期的に部隊の目標座標更新
void TankAI::Updatepoint() {

	float distance = GSvector3::distance(playerPosXZ_, attackPoint_);

	if (distance >= maxDistance_ || distance <= minDistance_) {

		attackPointFrag_ = false;

		DesignatedPoint();

		if (noPosition_)retreat();
		else {
			for (auto& tank : tanks_) {

				//死亡している個体や斬撃中の個体は除く
				if (tank->StateNow() == Tank::State::Die)continue;

				tank->AttackPoint(AttackPoint());
				tank->ChangeState(Tank::State::Move);
			}
		}
	}
	pointTimer_ = asignmentPointTimer_;

	updatePoint_ = false;
}

//部隊の死亡具合を知る
void TankAI::DieCheack(float timer) {
	for (auto& tank : tanks_) {

		if (tank->tag() == "DieEnemyTag") {
			dieCounter_++;
		}
	}

	//死亡した個体が２以上なら撤退
	if (dieCounter_ >= 2) {
		retreat();
		die_ = true;
	}

	if (dieCounter_ == MakeNumber) {

		for (auto& tank : tanks_) {
			//各タンクの死亡処理
			tank->die();
		}
		die_ = true;
	}
	dieCounter_ = 0;
}

//部隊の攻撃時の中心座標
void TankAI::DesignatedPoint() {

	while (!attackPointFrag_) {
		//プレイヤーに関する条件をクリアした座標を取得
		center_ = centerOfCircle();

		//配列内削除
		actors_.clear();

		//ワールド内にいるAI全取得
		actors_ = world_->find_actor_with_tag("EnemyAITag");

		//最も近い距離
		float nearDistance = 1000.0f;

		for (auto& actor : actors_) {

			//自身が生成した当たり判定を弾く
			if (actor == this || actor == NULL)continue;

			float distance = GSvector3::distance(center_, actor->transform().position());

			//最も近いやつを取得
			if (nearDistance > distance) {
				nearDistance = distance;
			}
		}

		//目的地に別の目的地が設定されていなかったら
		if (nearDistance > 10) {
			attackPointFrag_ = true;
			//中心座標更新
			attackPoint_ = center_;
			transform_.position(attackPoint_);
			designatedPointCounter_ = 0;
		}
		designatedPointCounter_++;
		//一定回数試行してもダメなら撤退
		if (designatedPointCounter_ >= 5) {
			attackPointFrag_ = true;
			noPosition_ = true;
		}
	}
}

//ランダムな円の中心座標を出す
GSvector3 TankAI::centerOfCircle() {

	// プレイヤー近くにランダムに移動させる
	float max = maxDistance_ - radius_;
	float min = minDistance_ + radius_;

	// プレイヤーの向きを基準にランダムな角度を生成
	float angle = gsRand(-randAngle_, randAngle_);

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
	float distance = gsRand(min, max);

	//ここでプレイヤーの高さに高い数値が入っている
	GSvector3 result = playerPosXZ_ + rotatedDirection * distance;

	// マップの端に抑える
	result.x = CLAMP(result.x, clampPosX_.x, clampPosX_.y);
	result.z = CLAMP(result.z, clampPosZ_.x, clampPosZ_.y);

	bool frag = PTRange(result);

	// プレイヤーの視界内なら座標を返し、視界外ならこの関数を再度呼び出す
	if (frag || nowCenterCompromiseCount_ >= centerCompromiseCount_) {
		nowCenterCompromiseCount_ = 0;

		//地面との交点を割り出した座標にする
		Ray ray = { result,-(transform_.up()) };
		GSvector3 intersect;
		world_->field()->collide(ray, result.y + rayLength_, &intersect);

		result.y = intersect.y;
		return result;
	}
	nowCenterCompromiseCount_++;
	return centerOfCircle();
}

//プレイヤー　ランダム　　戦車座標　　プレイヤー
bool TankAI::PTRange(GSvector3 pos) {

	//ランダム座標とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_Target = pos - playerPosXZ_;

	//プレイヤーの前ベクトルを求める
	GSvector3 forward = player_->transform().forward();

	//各ベクトルのy要素をなくす
	forward.y = to_Target.y = 0.0f;

	//2つのベクトルのなす角度を求める
	float angle = GSvector3::signedAngle(forward, to_Target);


	float distance = GSvector3::distance(pos, playerPosXZ_);

	//戦艦とプレイヤーの距離
	float shiptoPlayer = GSvector3::distance(enemyShip_->transform().position(), playerPos_);

	//指定角度内ならtrueを返し角度外ならfalseを返す
	return(angle <= 90 && angle >= -90 && distance >= minDistance_ && maxDistance_ >= distance && shiptoPlayer > minDistance_);
}

//攻撃ポイント 各個体の座標に使う
GSvector3 TankAI::AttackPoint() {

	GSvector3 attackpoint = GSvector3{ gsRand(-radius_,radius_) + attackPoint_.x,attackPoint_.y,gsRand(-radius_,radius_) + attackPoint_.z };

	float distance = GSvector3::distance(attackPoint_, attackpoint);

	if (distance <= radius_) {
		return attackpoint;
	}
	return AttackPoint();
}

void TankAI::attack() {

	//管理クラスから命令が下ったかつ自身がまだ攻撃処理をしていない場合
	if (aiAttackFrag_ && !attackFrag_) {

		//攻撃処理フラグを上げる
		attackFrag_ = true;

		//∀の個体に指示
		for (auto& tank : tanks_) {
			if (tank == NULL)continue;
			if (!tank->attackfrag() && !afterAttackFrag()) {
				tank->SetBullet();
				tank->setattackfrag(true);
			}
		}
	}

	//生存個体数と打ち切った個数を調べる
	for (auto& tank : tanks_) {
		//NULLならスキップ
		if (tank == NULL)continue;
		//弾切れしている固体をカウント
		if (tank->afterattackfrag())outOfBulletCounter_++;
		//生存している個体をカウント
		if (tank->StateNow() != Tank::State::Die && tank->StateNow() != Tank::State::RunAway) survivalCounter_++;
	}

	//生存している個体が弾を撃ち尽くしたら知らせる
	if (outOfBulletCounter_ == survivalCounter_) {

		for (auto& tank : tanks_) {
			//NULLならスキップ
			if (tank == NULL)continue;
			tank->setafterattackfrag(false);
		}
		aiAttackFrag_ = false;
		aiAfterAttackFrag_ = true;
		attackFrag_ = false;
	}
	outOfBulletCounter_ = 0;
	survivalCounter_ = 0;
}

void TankAI::setAttackFrag(bool frag) {
	aiAttackFrag_ = frag;
}

bool TankAI::attackFrag() const {
	return aiAttackFrag_;
}

void TankAI::setAfterAttackFrag(bool frag) {
	aiAfterAttackFrag_ = frag;
}

bool TankAI::afterAttackFrag()const {
	return aiAfterAttackFrag_;
}

//撤退
void TankAI::retreat() {

	//撤退フラグ
	retreatFrag_ = true;

	for (auto& tank : tanks_) {

		//死んでるやつには命令しない
		if (tank->StateNow() == Tank::State::Die)continue;

		//退却ポイントの設定
		GSvector3 shippos = enemyShip_->transform().position();
		Ray ray = { enemyShip_->transform().position(),-(transform_.up()) };
		GSvector3 intersect;
		world_->field()->collide(ray, enemyShip_->transform().position().y + rayLength_, &intersect);
		shippos.y = intersect.y;
		GSvector3 point = shippos;

		//自身を撤退ポイントに持っていく
		transform_.position(point);

		//戦車に撤退座標とステータス移行させる
		tank->AttackPoint(point);
		tank->ChangeState(Tank::State::RunAway);
	}
}

//戦艦に目標座標が無くて撤退しているのかを知らせる
bool TankAI::retreatFrag() const {
	return noPosition_;
}

//自身の死亡を知らせる
bool TankAI::dieTrigger() const {
	return die_;
}