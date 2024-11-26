#include "Mission.h"
#include "World/IWorld.h"
#include "Player/Player.h"
#include "BOSS/Boss.h"

const int MakeBossCounter_{ 1 };

Mission::Mission(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "MissionTag";
	name_ = "Mission";

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));



	state_ = State::Mission1;

}


void Mission::update(float delta_time) {

	if (boss_ == NULL) {
		boss_ = static_cast<Boss*>(world_->find_actor("Boss"));
	}

	//ステータスによってミッションの関数を呼ぶ
	switch (state_)
	{
	case Mission::State::Mission1:
		Mission1(delta_time);
		break;
	case Mission::State::Mission2:
		Mission2(delta_time);
		break;
	}

}

void Mission::draw() const {
	//ミッションの表示位置
	gsTextPos(800, 100);

	switch (state_)
	{
	case Mission::State::Mission1:
		gsDrawText("Mission1：敵を倒せ");
		gsTextPos(800, 150);
		gsDrawText("撃破数:%d", world_->gameData()->dieEnemyCounter());
		break;
	case Mission::State::Mission2:
		gsDrawText("Mission2：BOSSを倒せ");
		gsTextPos(800, 150);
		if (boss_ != NULL) {
			gsDrawText("BOSSのHP:%d", boss_->bossState_()->HP());
		}
		break;
	case Mission::State::GameClear:
		gsDrawText("GameClear");
		gsTextPos(800, 150);
		gsDrawText("ENTERを押してタイトルに戻る");
		break;
	}

}

void Mission::Mission1(float delta_time) {

	if (world_->gameData()->dieEnemyCounter() >= MakeBossCounter_) {
		world_->gameData()->setBossMake(true);
		state_ = State::Mission2;
	}

}

void Mission::Mission2(float delta_time) {

	if (world_->gameData()->bossDie() == true) {
		state_ = State::GameClear;
	}

}

void Mission::GameClear(float delta_time) {

}
