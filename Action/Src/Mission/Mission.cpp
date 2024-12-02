#include "Mission.h"
#include "World/IWorld.h"
#include "Player/Player.h"
#include "BOSS/Boss.h"

//ミッション１のノルマ
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
		mission1(delta_time);
		break;
	case Mission::State::Mission2:
		mission2(delta_time);
		break;
	case Mission::State::Mission3:
		MissionKillCounter_ = world_->gameData()->dieEnemyCounter() - beforKillCounter_;
		mission3(delta_time);
		break;
	case Mission::State::Mission4:
		mission4(delta_time);
		break;
	case Mission::State::GameClear:
		gameClear(delta_time);
		break;
	}

}

void Mission::draw() const {
}

//ミッション内容表示
void Mission::draw_gui() const {

	switch (state_)
	{
	case Mission::State::Mission1:

		if (world_->gameData()->dieEnemyCounter() < MakeBossCounter_) {
			gsTextPos(800, 100);
			gsDrawText("Mission1：敵を倒せ");
			gsTextPos(800, 150);
			gsDrawText("撃破数:%d", world_->gameData()->dieEnemyCounter());
		}
		else {
			gsTextPos(800, 100);
			gsDrawText("BOSSが出現した");
		}
		break;

	case Mission::State::Mission2:

		if (world_->gameData()->bossRetreat() == false) {
			gsTextPos(800, 100);
			gsDrawText("Mission2：BOSSを倒せ");
			gsTextPos(800, 150);

			if (boss_ != NULL) {
				gsDrawText("BOSSのHP:%d", boss_->bossState_()->HP());
			}
		}
		if (world_->gameData()->bossRetreat() == true) {
			gsTextPos(800, 100);
			gsDrawText("BOSSが撤退した");
			gsTextPos(800, 120);
			gsDrawText("このエリアを制圧するため、より多くの敵部隊を壊滅させよ");
		}
		break;

	case Mission::State::Mission3:
		if (MissionTimer > 0) {
			gsTextPos(800, 100);
			gsDrawText("より多くの敵部隊を壊滅させろ");
			gsTextPos(800, 150);
			gsDrawText("MissionTimer:　%d:%02d", (int)(MissionTimer / 3600), ((int)MissionTimer % 3600) / 60);
			gsTextPos(800, 170);
			gsDrawText("壊滅させた部隊数:%d", MissionKillCounter_);
		}
		else {
			gsTextPos(800, 100);
			gsDrawText("BOSSが再度出現した");
		}
		break;

	case Mission::State::Mission4:
		gsTextPos(800, 100);
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

void Mission::mission1(float delta_time) {

	//一定数殺したら
	if (world_->gameData()->dieEnemyCounter() >= MakeBossCounter_) {

		delay_timer -= delta_time;

		if (delay_timer <= 0) {
			delta_time = Assignmentdelay_timer;
			world_->gameData()->setBossMake(true);
			state_ = State::Mission2;
		}
	}
}

void Mission::mission2(float delta_time) {

	//ボスが退却したら
	if (world_->gameData()->bossRetreat()) {

		delay_timer -= delta_time;

		if (delay_timer <= 0) {
			delay_timer = Assignmentdelay_timer;
			//Mission3の時間
			MissionTimer = 7200.0f;
			//今までの退却させた部隊数
			beforKillCounter_ = world_->gameData()->dieEnemyCounter();
			state_ = State::Mission3;
		}
	}
}


void Mission::mission3(float delta_time) {

	//ミッション終了まで
	MissionTimer -= delta_time;

	//一定時間経ったら
	if (MissionTimer <= 0) {

		delay_timer -= delta_time;

		if (delay_timer <= 0) {
			delay_timer = Assignmentdelay_timer;
			world_->gameData()->setBossMake(true);
			state_ = State::Mission4;
		}
	}
}

void Mission::mission4(float delta_time) {

	if (world_->gameData()->bossDie()) {
		state_ = State::GameClear;
	}

}

void Mission::gameClear(float delta_time) {

}
