#include "Mission.h"
#include "World/IWorld.h"
#include "Player/Player.h"
#include "BOSS/Boss.h"
#include "Common/Assets.h"


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

	//背景描画
	static const GSvector2 Textureposition{ 500,0 };
	static const GSrect TextureRect{ 0,0,855,1078 };
	static const GSvector2 TextureScale{ 1.0,0.2 };
	static const GScolor4 textureColor{ 256,256,256,0.5f };
	gsDrawSprite2D(Texture_ResultBuck, &Textureposition, &TextureRect, NULL, &textureColor, &TextureScale, 0.0f);

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
			world_->gameData()->setMissionClear(1);
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
			world_->gameData()->setMissionClear(2);
			delay_timer = Assignmentdelay_timer;
			//Mission3の時間 7200
			MissionTimer = 200.0f;
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
			world_->gameData()->setMissionClear(3);

			delay_timer = Assignmentdelay_timer;
			world_->gameData()->setBossMake(true);
			float magnification;
			if (MissionKillCounter_ <= 10) {
				magnification = 1.5f;
			}
			else if (MissionKillCounter_ <= 20) {
				magnification = 2;
			}
			else if (MissionKillCounter_ <= 30) {
				magnification = 2.5f;
			}
			else if (MissionKillCounter_ <= 40) {
				magnification = 3;
			}
			else {
				magnification = 3.5f;
			}
			playerstateup(magnification);
			state_ = State::Mission4;
		}
	}
}

void Mission::mission4(float delta_time) {

	if (world_->gameData()->bossDie()) {

		world_->gameData()->setMissionClear(4);

		state_ = State::GameClear;
	}
}

void Mission::gameClear(float delta_time) {

}

//ミッションの成功報酬
void Mission::playerstateup(float magnification) {
	//maxhp
	player_->playerState_()->addMaxHP(player_->playerState_()->maxHP() * magnification);

	//HP
	player_->playerState_()->AddHP(player_->playerState_()->hp() * magnification);

	//攻撃
	player_->playerState_()->addAttack(player_->playerState_()->attack() * magnification);

	//防御
	player_->playerState_()->addDefense(player_->playerState_()->defense() * magnification);

	//スラスター
	player_->playerState_()->addMaxEnargy(player_->playerState_()->MaxEnargy() * magnification);
	player_->playerState_()->addEnargy(player_->playerState_()->MaxEnargy());
}