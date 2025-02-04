#include "EnemyBulletControl.h"
#include "BattleShip/EnemyShip.h"
#include "EnemyAI/HBMAI.h"
#include "EnemyAI/TankAI.h"
#include "World/IWorld.h"
#include <gslib.h>

//各配列数
const unsigned int BeamLifleAINumberOfArrays = 5;
const unsigned int GatlingAINumberOfArrays = 2;
const unsigned int TankAINumberOfArrays = 5;

//コンストラクタ
EnemyBulletControl::EnemyBulletControl(IWorld* world, const GSvector3& position) :
	BeamLifleAIs_{ BeamLifleAINumberOfArrays },
	GatlingAIs_{ GatlingAINumberOfArrays },
	TankAIs_{ TankAINumberOfArrays } {

	world_ = world;
	tag_ = "EnemyBulletControlTag";
	name_ = "EnemyBulletControl";

	transform_.position(position);
}

//デストラクタ
EnemyBulletControl::~EnemyBulletControl() {
	BeamLifleAIs_.clear();
	GatlingAIs_.clear();
	TankAIs_.clear();
}

//更新
void EnemyBulletControl::update(float delta_time) {

	//各攻撃命令関数呼ぶ
	attackBeamLifle(delta_time);
	attackGatling(delta_time);
	attackTanck(delta_time);
	//配列の管理
	sarch();
}

//ビームライフルAI配列に格納
void EnemyBulletControl::setBeamLifleAI(HBMAI* AI) {

	for (auto& BLAI : BeamLifleAIs_) {
		//配列内に空白があったらそこに入れる
		if (BLAI == NULL) {
			BLAI = AI;
			break;
		}
	}
}

//ガトリングAI配列に格納
void EnemyBulletControl::setGatlingAI(HBMAI* AI) {

	for (auto& GAI : GatlingAIs_) {
		//配列内に空白があったらそこに入れる
		if (GAI == NULL) {
			GAI = AI;
			break;
		}
	}
}

//戦車AI配列に格納
void EnemyBulletControl::setTanckAI(TankAI* AI) {
	for (auto& TAI : TankAIs_) {
		//配列内に空白があったらそこに入れる
		if (TAI == NULL) {
			TAI = AI;
			break;
		}
	}
}

//配列から排除
void EnemyBulletControl::sarch() {

	//戦車
	for (auto& TAI : TankAIs_) {
		if (TAI == NULL)continue;
		//タグが異なる場合もしくは撤退しているなら削除
		if (TAI->tag() != "EnemyAITag") {
			TAI = NULL;
			continue;
		}
		if (TAI->RetrunRetreatFrag())TAI = NULL;
	}
	//ビームライフル
	for (auto& BAI : BeamLifleAIs_) {
		if (BAI == NULL)continue;
		//タグが異なる場合削除
		if (BAI->tag() != "EnemyAITag") {
			BAI = NULL;
			continue;
		}
		if (BAI->RetrunRetreatFrag())BAI = NULL;
	}
	//ガトリング
	for (auto& GAI : GatlingAIs_) {
		if (GAI == NULL)continue;
		//タグが異なる場合削除
		if (GAI->tag() != "EnemyAITag") {
			GAI = NULL;
			continue;
		}
		if (GAI->RetrunRetreatFrag())GAI = NULL;
	}
}

//ビームライフル部隊に攻撃命令を出す
void EnemyBulletControl::attackBeamLifle(float delta_time) {

	//配列の始めを呼び出し
	hbmai1_ = BeamLifleAIs_[BeamLifleAICallNumber];
	//初めから３つ目の物を呼ぶ

	int callcounter = BeamLifleAICallNumber + 2;

	if (callcounter > BeamLifleAINumberOfArrays - 1) {
		callcounter -= BeamLifleAINumberOfArrays - 1;
	}
	hbmai2_ = BeamLifleAIs_[callcounter];

	if (hbmai1_ == NULL || hbmai1_->RetrunRetreatFrag()) hbmcall1 = true;
	if (hbmai2_ == NULL || hbmai2_->RetrunRetreatFrag()) hbmcall2 = true;

	if (hbmcall1) {
		//時間経過
		hbmprocessingTimer1 -= delta_time;
		//一定時間経過したら処理終了カウンターを加算
		if (hbmprocessingTimer1 <= 0) hbmcounter++;
	}
	else {
		if (!hbmai1_->afterattackfrag() && !hbmai1_->afterattackfrag()) {
			hbmai1_->setattackfrag(true);
		}
		//攻撃後なら処理終了カウンターを加算
		if (hbmai1_->afterattackfrag())hbmcounter++;

	}

	if (hbmcall2) {
		//時間経過
		hbmprocessingTimer2 -= delta_time;
		//一定時間経過したら処理終了カウンターを加算
		if (hbmprocessingTimer2 <= 0) hbmcounter++;
	}
	else {
		if (!hbmai2_->afterattackfrag() && !hbmai2_->afterattackfrag()) {
			hbmai2_->setattackfrag(true);
		}
		//攻撃後なら処理終了カウンターを加算
		if (hbmai2_->afterattackfrag())hbmcounter++;
	}

	//呼び出した個体の処理が終了したら呼び出す個体を更新
	if (hbmcounter == 2) {
		BeamLifleAttackTime -= delta_time;

		if (BeamLifleAttackTime <= 0) {
			BeamLifleAttackTime = 180.0f;
			if (!hbmcall1)hbmai1_->setafterattackfrag(false);
			if (!hbmcall2)hbmai2_->setafterattackfrag(false);
			BeamLifleAICallNumber++;
			hbmprocessingTimer1 = hbmprocessingTimer2 = 300.0f;
			hbmcall1 = hbmcall2 = false;
			if (BeamLifleAICallNumber > BeamLifleAINumberOfArrays - 1)BeamLifleAICallNumber = 0;
		}
	}
	else {
		hbmcounter = 0;
	}
}

//ガトリング部隊に攻撃命令を出す
void EnemyBulletControl::attackGatling(float delta_time) {

	//撤退中もしくは死んでいたら次の番号を呼び出す
	if (GatlingAIs_[GatringAICallNumber] == NULL || GatlingAIs_[GatringAICallNumber]->RetrunRetreatFrag()) {
		GatringAICallNumber++;
		if (GatringAICallNumber > GatlingAINumberOfArrays - 1) GatringAICallNumber = 0;
		return;
	}

	//攻撃命令出す
	if (!GatlingAIs_[GatringAICallNumber]->attackfrag() &&
		!GatlingAIs_[GatringAICallNumber]->afterattackfrag()) {
		GatlingAIs_[GatringAICallNumber]->setattackfrag(true);
	}

	if (GatlingAIs_[GatringAICallNumber]->afterattackfrag()) {

		GatringAttackTime -= delta_time;

		if (GatringAttackTime <= 0) {
			GatringAttackTime = 180.0f;
			GatlingAIs_[GatringAICallNumber]->setafterattackfrag(false);
			GatringAICallNumber++;
			if (GatringAICallNumber > GatlingAINumberOfArrays - 1) GatringAICallNumber = 0;
		}
	}
}

//戦車部隊に攻撃命令を出す
void EnemyBulletControl::attackTanck(float delta_time) {

	//配列の始めを呼び出し
	tankai1_ = TankAIs_[TankAICallNumber];
	//初めから３つ目の物を呼ぶ

	int callcounter = TankAICallNumber + 2;

	if (callcounter > TankAINumberOfArrays - 1) {
		callcounter -= TankAINumberOfArrays - 1;
	}
	tankai2_ = TankAIs_[callcounter];

	if (tankai1_ == NULL || tankai1_->RetrunRetreatFrag()) tankcall1 = true;
	if (tankai2_ == NULL || tankai2_->RetrunRetreatFrag()) tankcall2 = true;

	if (tankcall1) {
		//時間経過
		tankprocessingTimer1 -= delta_time;
		//一定時間経過したら処理終了カウンターを加算
		if (tankprocessingTimer1 <= 0) tankcounter++;
	}
	else {
		if (!tankai1_->afterattackfrag() && !tankai1_->afterattackfrag()) {
			tankai1_->setattackfrag(true);
		}
		//攻撃後なら処理終了カウンターを加算
		if (tankai1_->afterattackfrag())tankcounter++;

	}

	if (tankcall2) {
		//時間経過
		tankprocessingTimer2 -= delta_time;
		//一定時間経過したら処理終了カウンターを加算
		if (tankprocessingTimer2 <= 0) tankcounter++;
	}
	else {
		if (!tankai2_->afterattackfrag() && !tankai2_->afterattackfrag()) {
			tankai2_->setattackfrag(true);
		}
		//攻撃後なら処理終了カウンターを加算
		if (tankai2_->afterattackfrag())tankcounter++;
	}

	//呼び出した個体の処理が終了したら呼び出す個体を更新
	if (tankcounter == 2) {
		TankAttackTime -= delta_time;

		if (TankAttackTime <= 0) {
			TankAttackTime = 180.0f;
			if (!tankcall1)tankai1_->setafterattackfrag(false);
			if (!tankcall2)tankai2_->setafterattackfrag(false);
			TankAICallNumber++;
			tankprocessingTimer1 = tankprocessingTimer2 = 300.0f;
			tankcall1 = tankcall2 = false;
			if (TankAICallNumber > TankAINumberOfArrays - 1)TankAICallNumber = 0;
		}
	}
	else {
		tankcounter = 0;
	}

}