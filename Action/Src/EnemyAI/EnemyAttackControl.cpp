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

	if (world_->gameData()->missionClear() == 2) {
		//各攻撃命令関数呼ぶ
		//attackBeamLifleMission3(delta_time);
		//attackGatlingMission3(delta_time);
		//attackTanckMission3(delta_time);
	}
	else {
		//各攻撃命令関数呼ぶ
		attackBeamLifle(delta_time);
		//attackGatling(delta_time);
		attackTanck(delta_time);
	}
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
	hbmai1_ = beamLifleAIs_[beamLifleAICallNumber1_];
	hbmai2_ = beamLifleAIs_[beamLifleAICallNumber2_];

	//呼び出し固体がNULLもしくは撤退中なら
	if (hbmai1_ == NULL || hbmai1_->retreatFrag()) beamLifleNULL1_ = true;
	if (hbmai2_ == NULL || hbmai2_->retreatFrag()) beamLifleNULL2_ = true;

	if (beamLifleNULL1_) {
		beamLifleComple1_ = true;
	}
	else {
		if (!hbmai1_->afterAttackFrag())hbmai1_->setAttackFrag(true);
		else beamLifleComple1_ = true;
	}

	//二つ目の個体の処理
	if (beamLifleNULL2_) {
		beamLifleComple2_ = true;
	}
	else {
		if (!hbmai2_->afterAttackFrag())hbmai2_->setAttackFrag(true);
		else beamLifleComple2_ = true;
	}

	//呼び出した個体の処理が終了したら呼び出す個体を更新
	if (beamLifleComple1_ && beamLifleComple2_) {
		beamLifleAttackTime_ -= delta_time;

		if (beamLifleAttackTime_ <= 0) {
			beamLifleAttackTime_ = 180.0f;
			if (!beamLifleNULL1_)hbmai1_->setAfterAttackFrag(false);
			if (!beamLifleNULL2_)hbmai2_->setAfterAttackFrag(false);
			beamLifleNULL1_ = beamLifleNULL2_ = false;
			beamLifleComple1_ = beamLifleComple2_ = false;
			beamLifleCallComple_ = false;
		}
	}
}
//
////ガトリング部隊に攻撃命令を出す
//void EnemyAttackControl::attackGatling(float delta_time) {
//
//	//撤退中もしくは死んでいたら次の番号を呼び出す
//	if (gatlingAIs_[gatringAICallNumber_] == NULL) {
//		gatringAICallNumber_++;
//		if (gatringAICallNumber_ > GatlingAINumberOfArrays - 1) gatringAICallNumber_ = 0;
//		return;
//	}
//	if (gatlingAIs_[gatringAICallNumber_]->retreatFrag()) {
//		gatringAICallNumber_++;
//		if (gatringAICallNumber_ > GatlingAINumberOfArrays - 1) gatringAICallNumber_ = 0;
//		return;
//	}
//
//	//攻撃命令出す
//	if (!gatlingAIs_[gatringAICallNumber_]->attackfrag() &&
//		!gatlingAIs_[gatringAICallNumber_]->getattackfinishfrag()) {
//		gatlingAIs_[gatringAICallNumber_]->setattackfrag(true);
//	}
//
//	if (gatlingAIs_[gatringAICallNumber_]->getattackfinishfrag()) {
//
//		gatringAttackTime_ -= delta_time;
//
//		if (gatringAttackTime_ <= 0) {
//			gatringAttackTime_ = 180.0f;
//			gatlingAIs_[gatringAICallNumber_]->setafterattackfrag(false);
//			gatringAICallNumber_++;
//			if (gatringAICallNumber_ > GatlingAINumberOfArrays - 1) gatringAICallNumber_ = 0;
//		}
//	}
//}
//
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
//
//void EnemyAttackControl::attackBeamLifleMission3(float delta_time) {
//
//	//一つ目の呼び出し
//	hbmai1_ = beamLifleAIs_[beamLifleAICallNumber_];
//
//	//2つ目の呼び出し
//	int secondBeamLifleCallCounter_ = beamLifleAICallNumber_ + 2;
//	if (secondBeamLifleCallCounter_ > BeamLifleAINumberOfArraysM3 - 1) {
//		secondBeamLifleCallCounter_ -= BeamLifleAINumberOfArraysM3;
//	}
//	hbmai2_ = beamLifleAIs_[secondBeamLifleCallCounter_];
//
//	//3つ目の呼び出し
//	int ThirdBeamLifleCallCounter_ = beamLifleAICallNumber_ + 4;
//	if (ThirdBeamLifleCallCounter_ > BeamLifleAINumberOfArraysM3 - 1) {
//		ThirdBeamLifleCallCounter_ -= BeamLifleAINumberOfArraysM3;
//	}
//	hbmai3_ = beamLifleAIs_[ThirdBeamLifleCallCounter_];
//
//
//	//呼び出し固体がNULL又は退却中なら対象無しの処理に移行
//	if (hbmai1_ == NULL || hbmai1_->retreatFrag()) beamLifleNULL1_ = true;
//	if (hbmai2_ == NULL || hbmai2_->retreatFrag()) beamLifleNULL2_ = true;
//	if (hbmai3_ == NULL || hbmai3_->retreatFrag()) beamLifleCall3_ = true;
//
//	if (beamLifleNULL1_) {
//		//時間経過
//		hbmprocessingTimer1_ -= delta_time;
//		//一定時間経過したら処理終了カウンターを加算
//		if (hbmprocessingTimer1_ <= 0) hbmCounter_++;
//	}
//	else {
//		if (!hbmai1_->getattackfinishfrag() && !hbmai1_->getattackfinishfrag()) {
//			hbmai1_->setattackfrag(true);
//		}
//		//攻撃後なら処理終了カウンターを加算
//		if (hbmai1_->getattackfinishfrag())hbmCounter_++;
//	}
//
//	if (beamLifleNULL2_) {
//		//時間経過
//		hbmprocessingTimer2_ -= delta_time;
//		//一定時間経過したら処理終了カウンターを加算
//		if (hbmprocessingTimer2_ <= 0) hbmCounter_++;
//	}
//	else {
//		if (!hbmai2_->getattackfinishfrag() && !hbmai2_->getattackfinishfrag()) {
//			hbmai2_->setattackfrag(true);
//		}
//		//攻撃後なら処理終了カウンターを加算
//		if (hbmai2_->getattackfinishfrag())hbmCounter_++;
//	}
//
//	if (beamLifleCall3_) {
//		//時間経過
//		hbmprocessingTimer3_ -= delta_time;
//		//一定時間経過したら処理終了カウンターを加算
//		if (hbmprocessingTimer3_ <= 0) hbmCounter_++;
//	}
//	else {
//		if (!hbmai3_->getattackfinishfrag() && !hbmai3_->getattackfinishfrag()) {
//			hbmai3_->setattackfrag(true);
//		}
//		//攻撃後なら処理終了カウンターを加算
//		if (hbmai3_->getattackfinishfrag())hbmCounter_++;
//	}
//
//	//呼び出した個体の処理が終了したら呼び出す個体を更新
//	if (hbmCounter_ == 3) {
//		beamLifleAttackTime_ -= delta_time;
//
//		if (beamLifleAttackTime_ <= 0) {
//			beamLifleAttackTime_ = 180.0f;
//			if (!beamLifleNULL1_)hbmai1_->setafterattackfrag(false);
//			if (!beamLifleNULL2_)hbmai2_->setafterattackfrag(false);
//			if (!beamLifleCall3_)hbmai3_->setafterattackfrag(false);
//			beamLifleAICallNumber_++;
//			hbmprocessingTimer1_ = hbmprocessingTimer2_ = hbmprocessingTimer3_ = 300.0f;
//			beamLifleNULL1_ = beamLifleNULL2_ = beamLifleCall3_ = false;
//			if (beamLifleAICallNumber_ > BeamLifleAINumberOfArrays - 1)beamLifleAICallNumber_ = 0;
//		}
//	}
//	else hbmCounter_ = 0;
//
//}
//
//void EnemyAttackControl::attackGatlingMission3(float delta_time) {
//
//	//撤退中もしくは死んでいたら次の番号を呼び出す
//	if (gatlingAIs_[gatringAICallNumber_] == NULL) {
//		gatringAICallNumber_++;
//		if (gatringAICallNumber_ > GatlingAINumberOfArraysM3 - 1) gatringAICallNumber_ = 0;
//		return;
//	}
//	if (gatlingAIs_[gatringAICallNumber_]->retreatFrag()) {
//		gatringAICallNumber_++;
//		if (gatringAICallNumber_ > GatlingAINumberOfArraysM3 - 1) gatringAICallNumber_ = 0;
//		return;
//	}
//
//	//攻撃命令出す
//	if (!gatlingAIs_[gatringAICallNumber_]->attackfrag() &&
//		!gatlingAIs_[gatringAICallNumber_]->getattackfinishfrag()) {
//		gatlingAIs_[gatringAICallNumber_]->setattackfrag(true);
//	}
//
//	if (gatlingAIs_[gatringAICallNumber_]->getattackfinishfrag()) {
//
//		gatringAttackTime_ -= delta_time;
//
//		if (gatringAttackTime_ <= 0) {
//			gatringAttackTime_ = 180.0f;
//			gatlingAIs_[gatringAICallNumber_]->setafterattackfrag(false);
//			gatringAICallNumber_++;
//			if (gatringAICallNumber_ > GatlingAINumberOfArraysM3 - 1) gatringAICallNumber_ = 0;
//		}
//	}
//}
//
//void EnemyAttackControl::attackTanckMission3(float delta_time) {
//
//	//配列の始めを呼び出し
//	tankai1_ = tankAIs_[tankAICallNumber_];
//
//	//2つ目の呼び出し
//	int secondTankCallCounter = tankAICallNumber_ + 2;
//	if (secondTankCallCounter > TankAINumberOfArraysM3 - 1) {
//		secondTankCallCounter -= TankAINumberOfArraysM3;
//	}
//	tankai2_ = tankAIs_[secondTankCallCounter];
//
//	//3つ目の呼び出し
//	int thirdTankCallCounter = tankAICallNumber_ + 4;
//	if (thirdTankCallCounter > TankAINumberOfArraysM3 - 1) {
//		thirdTankCallCounter -= TankAINumberOfArraysM3;
//	}
//	tankai3_ = tankAIs_[thirdTankCallCounter];
//
//	//呼び出し相手が死亡または撤退中なら終了したことにする
//	if (tankai1_ == NULL || tankai1_->retreatFrag()) tankCall1_ = true;
//	if (tankai2_ == NULL || tankai2_->retreatFrag()) tankCall2_ = true;
//	if (tankai3_ == NULL || tankai3_->retreatFrag()) tankCall3_ = true;
//
//	if (tankCall1_) {
//		//時間経過
//		tankprocessingTimer1_ -= delta_time;
//		//一定時間経過したら処理終了カウンターを加算
//		if (tankprocessingTimer1_ <= 0) tankCounter_++;
//	}
//	else {
//		if (!tankai1_->afterattackfrag() && !tankai1_->afterattackfrag()) {
//			tankai1_->setattackfrag(true);
//		}
//		//攻撃後なら処理終了カウンターを加算
//		if (tankai1_->afterattackfrag())tankCounter_++;
//	}
//
//	if (tankCall2_) {
//		//時間経過
//		tankprocessingTimer2_ -= delta_time;
//		//一定時間経過したら処理終了カウンターを加算
//		if (tankprocessingTimer2_ <= 0) tankCounter_++;
//	}
//	else {
//		if (!tankai2_->afterattackfrag() && !tankai2_->afterattackfrag()) {
//			tankai2_->setattackfrag(true);
//		}
//		//攻撃後なら処理終了カウンターを加算
//		if (tankai2_->afterattackfrag())tankCounter_++;
//	}
//
//	if (tankCall3_) {
//		//時間経過
//		tankprocessingTimer3_ -= delta_time;
//		//一定時間経過したら処理終了カウンターを加算
//		if (tankprocessingTimer3_ <= 0) tankCounter_++;
//	}
//	else {
//		if (!tankai3_->afterattackfrag() && !tankai3_->afterattackfrag()) {
//			tankai3_->setattackfrag(true);
//		}
//		//攻撃後なら処理終了カウンターを加算
//		if (tankai3_->afterattackfrag())tankCounter_++;
//	}
//
//	//呼び出した個体の処理が終了したら呼び出す個体を更新
//	if (tankCounter_ == 3) {
//		tankAttackTime_ -= delta_time;
//
//		if (tankAttackTime_ <= 0) {
//			tankAttackTime_ = 180.0f;
//			if (!tankCall1_)tankai1_->setafterattackfrag(false);
//			if (!tankCall2_)tankai2_->setafterattackfrag(false);
//			if (!tankCall3_)tankai3_->setafterattackfrag(false);
//			tankAICallNumber_++;
//			tankprocessingTimer1_ = tankprocessingTimer2_ = tankprocessingTimer3_ = 300.0f;
//			tankCall1_ = tankCall2_ = tankCall3_ = false;
//			if (tankAICallNumber_ > TankAINumberOfArrays - 1)tankAICallNumber_ = 0;
//		}
//	}
//	else tankCounter_ = 0;
//}