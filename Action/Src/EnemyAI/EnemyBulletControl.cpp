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
	//attackBeamLifle(delta_time);
	attackGatling(delta_time);
	//attackTanck(delta_time);
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

//ビームライフル部隊に攻撃命令を出す
void EnemyBulletControl::attackBeamLifle(float delta_time) {

	//撤退中もしくは死亡しているなら次を呼び出す
	if (BeamLifleAIs_[BeamLifleAICallNumber]) {

	}

	//攻撃命令を出す

}

//ガトリング部隊に攻撃命令を出す
void EnemyBulletControl::attackGatling(float delta_time) {

	//撤退中もしくは死んでいたら次の番号を呼び出す
	if (GatlingAIs_[GatringAICallNumber] == NULL || GatlingAIs_[GatringAICallNumber]->RetrunRetreatFrag()) {
		GatringAICallNumber++;
		if (GatringAICallNumber > GatlingAINumberOfArrays - 1) GatringAICallNumber = 0;
		return;
	}

	bool attackfrag = GatlingAIs_[GatringAICallNumber]->attackfrag();

	bool afterfrag = GatlingAIs_[GatringAICallNumber]->afterattackfrag();

	//攻撃命令出す
	if (!attackfrag && !afterfrag) {
		GatlingAIs_[GatringAICallNumber]->setattackfrag(true);
	}

	if (afterfrag) {

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

	//撤退中もしくは死亡しているなら次を呼び出す
	if (TankAIs_[TankAICallNumber]) {

	}

	//攻撃命令を出す

}