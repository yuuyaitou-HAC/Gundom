#include "EnemyAttackControl.h"
#include "BattleShip/EnemyShip.h"
#include "EnemyAI/HBMAI.h"
#include "EnemyAI/TankAI.h"
#include "World/IWorld.h"
#include <gslib.h>
#include "Common/GameData.h"

//通常時の配列内の格納数
const unsigned int BeamRifleAINumberOfArrays = 5;
const unsigned int GatlingAINumberOfArrays = 2;
const unsigned int TankAINumberOfArrays = 5;

//ミッション３時の配列の格納数
const unsigned int BeamRifleAINumberOfArraysM3 = 10;
const unsigned int GatlingAINumberOfArraysM3 = 3;
const unsigned int TankAINumberOfArraysM3 = 10;

//コンストラクタ
EnemyAttackControl::EnemyAttackControl(IWorld* world, const GSvector3& position) :
	beamRifleAIs_{ BeamRifleAINumberOfArraysM3 },
	gatlingAIs_{ GatlingAINumberOfArraysM3 },
	tankAIs_{ TankAINumberOfArraysM3 } {

	world_ = world;
	tag_ = "EnemyBulletControlTag";
	name_ = "EnemyBulletControl";

	transform_.position(position);
}

//デストラクタ
EnemyAttackControl::~EnemyAttackControl() {
	beamRifleAIs_.clear();
	gatlingAIs_.clear();
	tankAIs_.clear();
}

//更新
void EnemyAttackControl::update(float delta_time) {
	//各攻撃命令関数呼ぶ
	attack_beamRifle(delta_time);
	attack_gatling(delta_time);
	attack_tank(delta_time);

	//配列の管理
	sarch();
}

//ビームライフルAI配列に格納
void EnemyAttackControl::set_beamRifleAI(HBMAI* AI) {

	for (auto& BLAI : beamRifleAIs_) {
		//配列内に空白があったらそこに入れる
		if (BLAI == NULL) {
			BLAI = AI;
			break;
		}
	}
}

//ガトリングAI配列に格納
void EnemyAttackControl::set_gatlingAI(HBMAI* AI) {

	for (auto& GAI : gatlingAIs_) {
		//配列内に空白があったらそこに入れる
		if (GAI == NULL) {
			GAI = AI;
			break;
		}
	}
}

//戦車AI配列に格納
void EnemyAttackControl::set_tankAI(TankAI* AI) {
	for (auto& TAI : tankAIs_) {
		//配列内に空白があったらそこに入れる
		if (TAI == NULL) {
			TAI = AI;
			break;
		}
	}
}

//配列から排除
void EnemyAttackControl::sarch() {

	//戦車
	for (auto& TAI : tankAIs_) {
		if (TAI == NULL)continue;
		//タグが異なる場合もしくは撤退しているなら削除
		if (TAI->tag() != "EnemyAITag") {
			TAI = NULL;
			continue;
		}
		if (TAI->retreatFrag())TAI = NULL;
	}
	//ビームライフル
	for (auto& BAI : beamRifleAIs_) {
		if (BAI == NULL)continue;
		//タグが異なる場合削除
		if (BAI->tag() != "EnemyAITag") {
			BAI = NULL;
			continue;
		}
		if (BAI->retreatFrag())BAI = NULL;
	}
	//ガトリング
	for (auto& GAI : gatlingAIs_) {
		if (GAI == NULL)continue;
		//タグが異なる場合削除
		if (GAI->tag() != "EnemyAITag") {
			GAI = NULL;
			continue;
		}
		if (GAI->retreatFrag())GAI = NULL;
	}
}

//ビームライフル部隊に攻撃命令を出す
void EnemyAttackControl::attack_beamRifle(float delta_time) {

	//呼び出し個体を被ることのないように呼び出す
	while (!beamRifleCallComple_) {
		beamRifleAICallNumber1_ = gsRand(0, BeamRifleAINumberOfArrays - 1);
		beamRifleAICallNumber2_ = gsRand(0, BeamRifleAINumberOfArrays - 1);
		//呼び出し番号が異なる場合
		if (beamRifleAICallNumber1_ != beamRifleAICallNumber2_)beamRifleCallComple_ = true;
	}

	//呼び出す個体を保存
	beamRifleAI1_ = beamRifleAIs_[beamRifleAICallNumber1_];
	beamRifleAI2_ = beamRifleAIs_[beamRifleAICallNumber2_];

	//呼び出し固体がNULLもしくは撤退中なら
	if (beamRifleAI1_ == NULL) {
		beamRifleNULL1_ = true;
	}
	else if (beamRifleAI1_->retreatFrag()) {
		beamRifleNULL1_ = true;
	}

	if (beamRifleAI2_ == NULL) {
		beamRifleNULL2_ = true;
	}
	else if (beamRifleAI2_->retreatFrag()) {
		beamRifleNULL2_ = true;
	}

	if (beamRifleNULL1_) {
		beamRifleComple_ = true;
	}
	else {
		if (!beamRifleAI1_->afterAttackFrag())beamRifleAI1_->setAttackFrag(true);
		else beamRifleComple_ = true;
	}

	//二つ目の個体の処理
	if (beamRifleNULL2_) {
		beamRifleComple2_ = true;
	}
	else {
		if (!beamRifleAI2_->afterAttackFrag())beamRifleAI2_->setAttackFrag(true);
		else beamRifleComple2_ = true;
	}

	//呼び出した個体の処理が終了したら呼び出す個体を更新
	if (beamRifleComple_ && beamRifleComple2_) {
		beamRifleAttackTime_ -= delta_time;

		if (beamRifleAttackTime_ <= 0) {
			beamRifleAttackTime_ = assignmentBeamRifleAttackTimer_;
			if (!beamRifleNULL1_)beamRifleAI1_->setAfterAttackFrag(false);
			if (!beamRifleNULL2_)beamRifleAI2_->setAfterAttackFrag(false);
			beamRifleNULL1_ = beamRifleNULL2_ = false;
			beamRifleComple_ = beamRifleComple2_ = false;
			beamRifleCallComple_ = false;
		}
	}
}

//ガトリング部隊に攻撃命令を出す
void EnemyAttackControl::attack_gatling(float delta_time) {

	if (gatlingAICallNumber_ > GatlingAINumberOfArrays - 1) {
		gatlingAICallNumber_ = 0;
	}
	//呼び出す個体を保存
	GatlingAI_ = gatlingAIs_[gatlingAICallNumber_];

	//呼び出し固体がNULLもしくは撤退中なら
	if (GatlingAI_ == NULL) {
		gatlingNULL_ = true;
	}
	else if (GatlingAI_->retreatFrag()) {
		gatlingNULL_ = true;
	}

	if (gatlingNULL_) {
		gatlingComple_ = true;
	}
	else {
		if (!GatlingAI_->afterAttackFrag())GatlingAI_->setAttackFrag(true);
		else gatlingComple_ = true;
	}

	//呼び出した個体の処理が終了したら呼び出す個体を更新
	if (gatlingComple_) {
		gatlingAttackTime_ -= delta_time;

		if (gatlingAttackTime_ <= 0) {
			gatlingAttackTime_ = 180.0f;
			if (!gatlingNULL_)GatlingAI_->setAfterAttackFrag(false);
			gatlingNULL_ = false;
			gatlingComple_ = false;
			gatlingAICallNumber_++;
		}
	}
}

//戦車部隊に攻撃命令を出す
void EnemyAttackControl::attack_tank(float delta_time) {
	//呼び出し個体を被ることのないように呼び出す
	while (!tankCallComple_) {
		tankAICallNumber1_ = gsRand(0, TankAINumberOfArrays - 1);
		tankAICallNumber2_ = gsRand(0, TankAINumberOfArrays - 1);
		//呼び出し番号が異なる場合
		if (tankAICallNumber1_ != tankAICallNumber2_)tankCallComple_ = true;
	}

	//呼び出す個体を保存
	tankai1_ = tankAIs_[tankAICallNumber1_];
	tankai2_ = tankAIs_[tankAICallNumber2_];

	//呼び出し固体がNULLもしくは撤退中なら
	if (tankai1_ == NULL) {
		tankNULL1_ = true;
	}
	else if (tankai1_->retreatFrag()) {
		tankNULL1_ = true;
	}

	if (tankai2_ == NULL) {
		tankNULL2_ = true;
	}
	else if (tankai2_->retreatFrag()) {
		tankNULL2_ = true;
	}

	if (tankNULL1_) {
		tankComple1_ = true;
	}
	else {
		if (!tankai1_->afterAttackFrag())tankai1_->setAttackFrag(true);
		else tankComple1_ = true;
	}

	//二つ目の個体の処理
	if (tankNULL2_) {
		tankComple2_ = true;
	}
	else {
		if (!tankai2_->afterAttackFrag()) tankai2_->setAttackFrag(true);
		else tankComple2_ = true;
	}

	//呼び出した個体の処理が終了したら呼び出す個体を更新
	if (tankComple1_ && tankComple2_) {
		tankLifleAttackTime_ -= delta_time;

		if (tankLifleAttackTime_ <= 0) {
			tankLifleAttackTime_ = 180.0f;
			if (!tankNULL1_)tankai1_->setAfterAttackFrag(false);
			if (!tankNULL2_)tankai2_->setAfterAttackFrag(false);
			tankNULL1_ = tankNULL2_ = false;
			tankComple1_ = tankComple2_ = false;
			tankCallComple_ = false;
		}
	}
}