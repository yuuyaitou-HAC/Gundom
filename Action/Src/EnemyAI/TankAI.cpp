#include "TankAI.h"
#include "Enemy/Tank.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Player/Player.h"
#include <gslib.h>
#include "BattleShip/EnemyShip.h"
#include "Collision/Ray.h"

//生成数
int MakeNumber = 5;

TankAI::TankAI(IWorld* world, const GSvector3& position) :
	tanks_(MakeNumber) {

	world_ = world;

	tag_ = "EnemyAITag";

	name_ = "TankAI";

	transform_.position(position);

	//プレイヤー取得
	player = static_cast<Player*>(world_->find_actor("Player"));

	//自身の戦艦を取得
	enemyship = static_cast<EnemyShip*>(world_->find_actor("EnemyShip"));

	//戦車の生成
	MakeTank();

	//部隊の距離
	MinDistance = 25;
	MaxDistance = 40;
}

TankAI::~TankAI() {

	tanks_.clear();
	if (cd_ != NULL)	cd_->die();
}

void TankAI::MakeTank() {

	//生成座標に自身の座標を代入
	makepos = transform_.position();

	//生成数分戦車を生成
	for (int i = 0; i < MakeNumber; i++) {
		tanks_[i] = new Tank{ world_,makepos };
		world_->add_actor(tanks_[i]);
		makepos.x += 2;
	}
}

void TankAI::update(float delta_time) {

	//時間による制御
	MoveTimer += delta_time;

	pointtimer -= delta_time;

	Playerpos = player->transform().position();

	playerposxz = Playerpos;
	playerposxz.y = -11.3;

	if (!noposition || !Die) {

		if (pointtimer <= 0) {
			updatepoint = true;
			Updatepoint();
		}

		if (!updatepoint)MovePoint();
	}

	//戦車の死亡判定
	DieCheack(delta_time);
}

void TankAI::draw() const {
}

bool TankAI::MoveTrigger() {
	//各戦車が移動中かどうか
	for (auto& tank : tanks_) {

		if (tank->StateNow() == 2) {
			return true;
		}
	}
	return false;
}

void TankAI::MovePoint() {

	//一定時間経過かつ移動中フラグがなければ
	if (MoveTimer >= 180 && !MoveTrigger()) {

		for (auto& tank : tanks_) {

			//死亡している個体はスキップ
			if (tank->StateNow() == 6)continue;

			//タンク座標取得
			TanksPos = tank->transform().position();

			//プレイヤーとタンクの距離を取る
			PlayerToTank = GSvector3::distance(playerposxz, TanksPos);

			if (far < PlayerToTank)far = PlayerToTank;
			if (close > PlayerToTank)close = PlayerToTank;

		}
		//距離が一定以内なら移動開始
		if (far > MaxDistance || close < MinDistance) {

			AttackPointFrag_ = false;

			//当たり判定二生成と部隊の移動すべき座標を取得
			DesignatedPoint();

			if (noposition)	retreat();
			else {
				for (auto& tank : tanks_) {

					//死亡している個体はスキップ
					if (tank->StateNow() == 6)continue;

					tank->AttackPoint(AttackPoint());
					tank->ChangeState(2);
				}
			}
		}
		MoveTimer = 0;
		far = 0;
		close = 1000;
	}
}

//定期的に部隊の目標座標更新
void TankAI::Updatepoint() {

	float distance = GSvector3::distance(playerposxz, attackPoint_);

	if (distance >= MaxDistance || distance <= MinDistance) {

		AttackPointFrag_ = false;

		DesignatedPoint();

		if (noposition)retreat();
		else {
			for (auto& tank : tanks_) {

				//死亡している個体や斬撃中の個体は除く
				if (tank->StateNow() == 6)continue;

				tank->AttackPoint(AttackPoint());
				tank->ChangeState(2);
			}
		}
	}
	pointtimer = asignmentpointtimer;

	updatepoint = false;
}

//部隊の死亡具合を知る
void TankAI::DieCheack(float timer) {
	for (auto& tank : tanks_) {

		if (tank->StateNow() == 6) {
			DieCounter++;
		}
	}

	//死亡した個体が２以上なら撤退
	if (DieCounter >= 2) {
		retreat();
		Die = true;
	}
	//全滅したら各戦車を死亡させて自身も死ぬ
	if (DieCounter == MakeNumber) {

		for (auto& tank : tanks_) {
			//各タンクの死亡処理
			tank->die();
		}
		DieAI = true;
	}
	DieCounter = 0;
}

//部隊の攻撃時の中心座標
void TankAI::DesignatedPoint() {

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
			//中心座標更新
			attackPoint_ = center;
			//ほかの部隊の目的地になっていないかを調べるための当たり判定を生成
			cd_ = new CollisionDerection{ world_,attackPoint_,"CollisionDerectionTag",radius };
			world_->add_actor(cd_);
			DesignatedPointcounter = 0;
		}
		DesignatedPointcounter++;
		//一定回数試行してもダメなら撤退
		if (DesignatedPointcounter >= 5) {
			AttackPointFrag_ = true;
			noposition = true;
		}
	}
}

//ランダムな円の中心座標を出す
GSvector3 TankAI::centerOfCircle() {

	// プレイヤー近くにランダムに移動させる
	float max = MaxDistance - radius;
	float min = MinDistance + radius;

	// プレイヤーの向きを基準にランダムな角度を生成
	float angle = gsRand(-90.0f, 90.0f);

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
	float distance = gsRand(min, max);

	//y軸をマップと設置しているときのものにする
	GSvector3 playerposxz = Playerpos;
	//マップの最低値に設定
	playerposxz.y = -11.3f;

	//ここでプレイヤーの高さに高い数値が入っている
	GSvector3 result = playerposxz + rotatedDirection * distance;

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

//プレイヤー　ランダム　　戦車座標　　プレイヤー
bool TankAI::PTRange(GSvector3 pos) {

	//ランダム座標とプレイヤーの座標の方向ベクトルを求める
	GSvector3 to_Target = pos - playerposxz;

	//プレイヤーの前ベクトルを求める
	GSvector3 forward = player->transform().forward();

	//各ベクトルのy要素をなくす
	forward.y = to_Target.y = 0.0f;

	//2つのベクトルのなす角度を求める
	float angle = GSvector3::signedAngle(forward, to_Target);


	float distance = GSvector3::distance(pos, playerposxz);

	//戦艦とプレイヤーの距離
	float shiptoPlayer = GSvector3::distance(enemyship->transform().position(), Playerpos);

	//指定角度内ならtrueを返し角度外ならfalseを返す
	return(angle <= 90 && angle >= -90 && distance >= MinDistance && MaxDistance >= distance && shiptoPlayer > MinDistance);
}

//攻撃ポイント 各個体の座標に使う
GSvector3 TankAI::AttackPoint() {

	GSvector3 attackpoint = GSvector3{ gsRand(-radius,radius) + attackPoint_.x,attackPoint_.y,gsRand(-radius,radius) + attackPoint_.z };

	float distance = GSvector3::distance(attackPoint_, attackpoint);

	if (distance <= radius) {
		return attackpoint;
	}
	return AttackPoint();
}

//撤退
void TankAI::retreat() {

	for (auto& tank : tanks_) {

		//死んでるやつには命令しない
		if (tank->StateNow() == 6)continue;

		//退却ポイントの設定
		GSvector3 shippos = enemyship->transform().position();
		Ray ray = { enemyship->transform().position(),-(transform_.up()) };
		GSvector3 intersect;
		world_->field()->collide(ray, enemyship->transform().position().y + 30.0f, &intersect);
		shippos.y = intersect.y;
		GSvector3 point = shippos;

		tank->AttackPoint(point);
		tank->ChangeState(5);
	}
}

//戦艦に目標座標が無くて撤退しているのかを知らせる
bool TankAI::retreatFrag() {
	return noposition;
}

//自身の死亡を知らせる
bool TankAI::dieTrigger() {
	return DieAI;
}