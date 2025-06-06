#include "EnemyAttackControl.h"
#include "BattleShip/EnemyShip.h"
#include "EnemyAI/HBMAI.h"
#include "EnemyAI/TankAI.h"
#include "World/IWorld.h"
#include <gslib.h>
#include "Common/GameData.h"

//通常時の配列内の格納数
const unsigned int BeamLifleAINumberOfArrays = 5;
const unsigned int GatlingAINumberOfArrays = 2;
const unsigned int TankAINumberOfArrays = 5;

//ミッション３時の配列の格納数
const unsigned int BeamLifleAINumberOfArraysM3 = 10;
const unsigned int GatlingAINumberOfArraysM3 = 3;
const unsigned int TankAINumberOfArraysM3 = 10;

//コンストラクタ
EnemyAttackControl::EnemyAttackControl(IWorld* world, const GSvector3& position) :
	beamLifleAIs_{ BeamLifleAINumberOfArraysM3 },
	gatlingAIs_{ GatlingAINumberOfArraysM3 },
	tankAIs_{ TankAINumberOfArraysM3 } {

	world_ = world;
	tag_ = "EnemyBulletControlTag";
	name_ = "EnemyBulletControl";

	transform_.position(position);
}

//デストラクタ
EnemyAttackControl::~EnemyAttackControl() {
	beamLifleAIs_.clear();
	gatlingAIs_.clear();
	tankAIs_.clear();
}

//更新
void EnemyAttackControl::update(float delta_time) {


	//各攻撃命令関数呼ぶ
	attackBeamLifle(delta_time);
	attackGatling(delta_time);
	attackTanck(delta_time);

	//配列の管理
	sarch();
}

//ビームライフルAI配列に格納
void EnemyAttackControl::setBeamLifleAI(HBMAI* AI) {

	for (auto& BLAI : beamLifleAIs_) {
		//配列内に空白があったらそこに入れる
		if (BLAI == NULL) {
			BLAI = AI;
			break;
		}
	}
}

//ガトリングAI配列に格納
void EnemyAttackControl::setGatlingAI(HBMAI* AI) {

	for (auto& GAI : gatlingAIs_) {
		//配列内に空白があったらそこに入れる
		if (GAI == NULL) {
			GAI = AI;
			break;
		}
	}
}

//戦車AI配列に格納
void EnemyAttackControl::setTanckAI(TankAI* AI) {
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
	for (auto& BAI : beamLifleAIs_) {
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
void EnemyAttackControl::attackBeamLifle(float delta_time) {

	//呼び出し個体を被ることのないように呼び出す
	while (!beamLifleCallComple_) {
		beamLifleAICallNumber1_ = gsRand(0, BeamLifleAINumberOfArrays - 1);
		beamLifleAICallNumber2_ = gsRand(0, BeamLifleAINumberOfArrays - 1);
		//呼び出し番号が異なる場合
		if (beamLifleAICallNumber1_ != beamLifleAICallNumber2_)beamLifleCallComple_ = true;
	}

	//呼び出す個体を保存
	beamRifleAI1_ = beamLifleAIs_[beamLifleAICallNumber1_];
	beamRifleAI2_ = beamLifleAIs_[beamLifleAICallNumber2_];

	//呼び出し固体がNULLもしくは撤退中なら
	if (beamRifleAI1_ == NULL || beamRifleAI1_->retreatFrag()) beamLifleNULL1_ = true;
	if (beamRifleAI2_ == NULL || beamRifleAI2_->retreatFrag()) beamLifleNULL2_ = true;

	if (beamLifleNULL1_) {
		beamLifleComple_ = true;
	}
	else {
		if (!beamRifleAI1_->afterAttackFrag())beamRifleAI1_->setAttackFrag(true);
		else beamLifleComple_ = true;
	}

	//二つ目の個体の処理
	if (beamLifleNULL2_) {
		beamLifleComple2_ = true;
	}
	else {
		if (!beamRifleAI2_->afterAttackFrag())beamRifleAI2_->setAttackFrag(true);
		else beamLifleComple2_ = true;
	}

	//呼び出した個体の処理が終了したら呼び出す個体を更新
	if (beamLifleComple_ && beamLifleComple2_) {
		beamLifleAttackTime_ -= delta_time;

		if (beamLifleAttackTime_ <= 0) {
			beamLifleAttackTime_ = 180.0f;
			if (!beamLifleNULL1_)beamRifleAI1_->setAfterAttackFrag(false);
			if (!beamLifleNULL2_)beamRifleAI2_->setAfterAttackFrag(false);
			beamLifleNULL1_ = beamLifleNULL2_ = false;
			beamLifleComple_ = beamLifleComple2_ = false;
			beamLifleCallComple_ = false;
		}
	}
}

//ガトリング部隊に攻撃命令を出す
void EnemyAttackControl::attackGatling(float delta_time) {

	if (gatringAICallNumber_ > GatlingAINumberOfArrays - 1) {
		gatringAICallNumber_ = 0;
	}
	//呼び出す個体を保存
	GatringAI_ = gatlingAIs_[gatringAICallNumber_];

	//呼び出し固体がNULLもしくは撤退中なら
	if (GatringAI_ == NULL || GatringAI_->retreatFrag()) gatringNULL_ = true;

	if (gatringNULL_) {
		gatringComple_ = true;
	}
	else {
		if (!GatringAI_->afterAttackFrag())GatringAI_->setAttackFrag(true);
		else gatringComple_ = true;
	}

	//呼び出した個体の処理が終了したら呼び出す個体を更新
	if (gatringComple_) {
		gatringAttackTime_ -= delta_time;

		if (gatringAttackTime_ <= 0) {
			gatringAttackTime_ = 180.0f;
			if (!gatringNULL_)GatringAI_->setAfterAttackFrag(false);
			gatringNULL_ = false;
			gatringComple_ = false;
			gatringAICallNumber_++;
		}
	}
}

//戦車部隊に攻撃命令を出す
void EnemyAttackControl::attackTanck(float delta_time) {
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
	if (tankai1_ == NULL || tankai1_->retreatFrag()) tankNULL1_ = true;
	if (tankai2_ == NULL || tankai2_->retreatFrag()) tankNULL2_ = true;


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