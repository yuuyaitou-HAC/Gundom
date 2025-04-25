#include "Mission.h"
#include "World/IWorld.h"
#include "Player/Player.h"
#include "UnderBoss/UnderBoss.h"
#include "Common/Assets.h"

#include "imgui/imgui.h"
//ミッション１のノルマ
const int MakeBossCounter_{ 0 };

Mission::Mission(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "MissionTag";
	name_ = "Mission";

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	state_ = State::Mission3;

	missionTimer_ = 6000.0f;

}

void Mission::update(float delta_time) {

	//中ボス取得できていなかったら取得する
	if (underBoss_ == NULL) {
		underBoss_ = static_cast<UnderBoss*>(world_->find_actor("UnderBoss"));
	}

	//ボス取得できていなかったら取得する
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
		if (underBoss_ != NULL) {
			float maxhp = underBoss_->underBossState_()->MaxHP();
			float hp = underBoss_->underBossState_()->HP();
			hpBarScale_ = (maxhp - hp) / maxhp;
			hpBarScale_ = CLAMP(hpBarScale_, 0, 1);
		}
		mission2(delta_time);
		break;
	case Mission::State::Mission3:
		missionKillCounter_ = world_->gameData()->dieEnemyCounter() - beforKillCounter_;
		mission3(delta_time);
		break;
	case Mission::State::Mission4:
		if (boss_ != NULL) {
			float maxhp = boss_->bossState_()->MaxHP();
			float hp = boss_->bossState_()->HP();
			hpBarScale_ = (maxhp - hp) / maxhp;
			hpBarScale_ = CLAMP(hpBarScale_, 0, 1);
		}
		mission4(delta_time);
		break;
	case Mission::State::GameClear:
		gameClear(delta_time);
		break;
	}
}

//ミッション内容表示
void Mission::draw_gui() const {

	//背景描画

	gsDrawSprite2D(Texture_MissionBack, &missionBackPosition_, &missionBackRect_, NULL, &textureColor_, &missionBackScale_, 0.0f);

	switch (state_)
	{
	case Mission::State::Mission1:

		if (world_->gameData()->dieEnemyCounter() < MakeBossCounter_) {

			//ミッション内容
			gsDrawSprite2D(Texture_Mission1, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);

			//撃破数
			gsDrawSprite2D(Texture_KillNum, &killnumPosition_, &killnumRect_, NULL, &textureColor_, &killnumScale_, 0.0f);

			//現在の撃破数
			if (world_->gameData()->dieEnemyCounter() >= 10) {
				tens = numRect_[world_->gameData()->dieEnemyCounter() / 10];
				ones = numRect_[world_->gameData()->dieEnemyCounter() % 10];

				numPos_ = GSvector2{ 900,140 };
				gsDrawSprite2D(Texture_Number, &numPos_, &tens, NULL, &textureColor_, &numScale_, 0.0f);
				numPos_ = GSvector2{ 950,140 };
				gsDrawSprite2D(Texture_Number, &numPos_, &ones, NULL, &textureColor_, &numScale_, 0.0f);

			}
			else {
				ones = numRect_[world_->gameData()->dieEnemyCounter()];
				numPos_ = GSvector2{ 950,140 };
				gsDrawSprite2D(Texture_Number, &numPos_, &ones, NULL, &textureColor_, &numScale_, 0.0f);
			}

			//スラッシュ
			gsDrawSprite2D(Texture_Slash, &slashPosition_, &slashRect_, NULL, &textureColor_, &slashScale_, 0.0f);

			//目標撃破数
			if (MakeBossCounter_ >= 10) {
				tens = numRect_[MakeBossCounter_ / 10];
				ones = numRect_[MakeBossCounter_ % 10];

				numPos_ = GSvector2{ 1050,140 };
				gsDrawSprite2D(Texture_Number, &numPos_, &tens, NULL, &textureColor_, &numScale_, 0.0f);
				numPos_ = GSvector2{ 1100,140 };
				gsDrawSprite2D(Texture_Number, &numPos_, &ones, NULL, &textureColor_, &numScale_, 0.0f);

			}
			else {
				ones = numRect_[MakeBossCounter_];
				numPos_ = GSvector2{ 1050,140 };
				gsDrawSprite2D(Texture_Number, &numPos_, &ones, NULL, &textureColor_, &numScale_, 0.0f);
			}
		}
		else {
			//中ボス出現
			gsDrawSprite2D(Texture_UnderBossadvent, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);
		}
		break;

	case Mission::State::Mission2:

		if (world_->gameData()->underBossDie() == false) {

			//ミッション内容
			gsDrawSprite2D(Texture_Mission2, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);

			gsTextPos(800, 150);

			if (underBoss_ != NULL) {

				//体力バー
				//HPバー(青)
				gsDrawSprite2D(Texture_HP, &hpposition_, &hpRect_,
					NULL, &textureColor_, &hpScale_, 0.0f);

				GSvector2 HPBackScale{ hpBarScale_,2.5 };
				gsDrawSprite2D(Texture_HPBack, &hpBackposition_, &hpBackRect_,
					NULL, &textureColor_, &HPBackScale, 180.0f);

				//現在の中ボスのHP

				hundreds = numRect_[underBoss_->underBossState_()->HP() / 100];
				tens = numRect_[(underBoss_->underBossState_()->HP() / 10) % 10];
				ones = numRect_[underBoss_->underBossState_()->HP() % 10];
				numPosHP_ = GSvector2{ 905,100 };
				gsDrawSprite2D(Texture_Number, &numPosHP_, &hundreds, NULL, &textureColor_, &numScaleHP_, 0.0f);
				numPosHP_ = GSvector2{ 930,100 };
				gsDrawSprite2D(Texture_Number, &numPosHP_, &tens, NULL, &textureColor_, &numScaleHP_, 0.0f);
				numPosHP_ = GSvector2{ 955,100 };
				gsDrawSprite2D(Texture_Number, &numPosHP_, &ones, NULL, &textureColor_, &numScaleHP_, 0.0f);

				//スラッシュ
				gsDrawSprite2D(Texture_Slash, &slashPositionHP_, &slashRectHP_, NULL, &textureColor_, &slashScaleHP_, 0.0f);

				//マックス時の中ボスのHP
				hundreds = numRect_[underBoss_->underBossState_()->MaxHP() / 100];
				tens = numRect_[(underBoss_->underBossState_()->MaxHP() / 10) % 10];
				ones = numRect_[underBoss_->underBossState_()->MaxHP() % 10];
				numPosHP_ = GSvector2{ 1005,100 };
				gsDrawSprite2D(Texture_Number, &numPosHP_, &hundreds, NULL, &textureColor_, &numScaleHP_, 0.0f);
				numPosHP_ = GSvector2{ 1030,100 };
				gsDrawSprite2D(Texture_Number, &numPosHP_, &tens, NULL, &textureColor_, &numScaleHP_, 0.0f);
				numPosHP_ = GSvector2{ 1055,100 };
				gsDrawSprite2D(Texture_Number, &numPosHP_, &ones, NULL, &textureColor_, &numScaleHP_, 0.0f);

				//中ボスのHP説明
				gsDrawSprite2D(Texture_UnderBossHP, &ubHPPosition_, &ubHPRect_, NULL, &textureColor_, &ubHPScale_, 0.0f);
			}
		}
		if (world_->gameData()->underBossDie() == true) {

			//中ボス死亡
			gsDrawSprite2D(Texture_UnderBossKill, &ubKillPosition_, &ubKillRect_, NULL, &textureColor_, &ubKillScale_, 0.0f);


			//ミッション３導入
			gsDrawSprite2D(Texture_MissionDescription, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);
		}
		break;

	case Mission::State::Mission3:
		if (missionTimer_ > 0) {

			//ミッション内容
			gsDrawSprite2D(Texture_Mission3, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);

			gsTextPos(800, 150);
			gsDrawText("MissionTimer:　%d:%02d", (int)(missionTimer_ / 3600), ((int)missionTimer_ % 3600) / 60);

			gsDrawSprite2D(Texture_missiontimer, &mtPosition_, &mtRect_, NULL, &textureColor_, &mtScale_, 0.0f);


			tens = numRect_[missionKillCounter_ / 10];
			ones = numRect_[missionKillCounter_ % 10];
			numPosHP_ = GSvector2{ 1110,190 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &tens, NULL, &textureColor_, &mission3NumScale_, 0.0f);
			numPosHP_ = GSvector2{ 1150,190 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &ones, NULL, &textureColor_, &mission3NumScale_, 0.0f);

			gsDrawSprite2D(Texture_Killnum2, &killnum2Position_, &killnum2Rect_, NULL, &textureColor_, &killnum2Scale_, 0.0f);

		}
		else {
			//BOSS登場
			gsDrawSprite2D(Texture_BossMake, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);
		}
		break;

	case Mission::State::Mission4:

		//ミッション内容
		gsDrawSprite2D(Texture_Mission4, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);

		gsTextPos(800, 150);
		if (boss_ != NULL) {

			//体力バー
			gsDrawSprite2D(Texture_HP, &hpposition_, &hpRect_, NULL, &textureColor_, &hpScale_, 0.0f);

			GSvector2 HPBackScale{ hpBarScale_,2.5 };
			gsDrawSprite2D(Texture_HPBack, &hpBackposition_, &hpBackRect_, NULL, &textureColor_, &HPBackScale, 180.0f);

			//現在のHP
			thousand = numRect_[boss_->bossState_()->HP() / 1000];
			hundreds = numRect_[(boss_->bossState_()->HP() / 100) % 10];
			tens = numRect_[(boss_->bossState_()->HP() / 10) % 10];
			ones = numRect_[boss_->bossState_()->HP() % 10];
			numPosHP_ = GSvector2{ 880,100 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &thousand, NULL, &textureColor_, &numScaleHP_, 0.0f);
			numPosHP_ = GSvector2{ 905,100 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &hundreds, NULL, &textureColor_, &numScaleHP_, 0.0f);
			numPosHP_ = GSvector2{ 930,100 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &tens, NULL, &textureColor_, &numScaleHP_, 0.0f);
			numPosHP_ = GSvector2{ 955,100 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &ones, NULL, &textureColor_, &numScaleHP_, 0.0f);

			//スラッシュ
			gsDrawSprite2D(Texture_Slash, &slashPositionHP_, &slashRectHP_, NULL, &textureColor_, &slashScaleHP_, 0.0f);

			//マックス時のボスのHP
			thousand = numRect_[boss_->bossState_()->MaxHP() / 1000];
			hundreds = numRect_[(boss_->bossState_()->MaxHP() / 100) % 10];
			tens = numRect_[(boss_->bossState_()->MaxHP() / 10) % 10];
			ones = numRect_[boss_->bossState_()->MaxHP() % 10];
			numPosHP_ = GSvector2{ 1005,100 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &thousand, NULL, &textureColor_, &numScaleHP_, 0.0f);
			numPosHP_ = GSvector2{ 1030,100 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &hundreds, NULL, &textureColor_, &numScaleHP_, 0.0f);
			numPosHP_ = GSvector2{ 1055,100 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &tens, NULL, &textureColor_, &numScaleHP_, 0.0f);
			numPosHP_ = GSvector2{ 1080,100 };
			gsDrawSprite2D(Texture_Number, &numPosHP_, &ones, NULL, &textureColor_, &numScaleHP_, 0.0f);

			//ボスのHP説明
			gsDrawSprite2D(Texture_bossHP, &ubHPPosition_, &ubHPRect_, NULL, &textureColor_, &ubHPScale_, 0.0f);
		}
		break;

	case Mission::State::GameClear:

		//ゲームクリア
		gsDrawSprite2D(Texture_GameClear, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);
		//エンターキー
		gsDrawSprite2D(Texture_Enter, &enterPos_, &enterRect_, NULL, &textureColor_, &enterScal_, 0.0f);

		break;
	}
}

void Mission::mission1(float delta_time) {

	//一定数殺したら
	if (world_->gameData()->dieEnemyCounter() >= MakeBossCounter_) {

		delayTimer_ -= delta_time;

		if (delayTimer_ <= 0) {
			world_->gameData()->setMissionClear(1);
			delayTimer_ = assignmentdelayTimer_;
			world_->gameData()->setUnderBossMake(true);
			state_ = State::Mission2;
		}
	}
}

void Mission::mission2(float delta_time) {

	//ボス死んだら
	if (world_->gameData()->underBossDie()) {

		delayTimer_ -= delta_time;

		if (delayTimer_ <= 0) {
			world_->gameData()->setMissionClear(2);
			delayTimer_ = assignmentdelayTimer_;
			//Mission3の時間 7200
			//missionTimer_ = 3600.0f;
			//今までの退却させた部隊数
			beforKillCounter_ = world_->gameData()->dieEnemyCounter();
			state_ = State::Mission3;
		}
	}
}

void Mission::mission3(float delta_time) {

	//ミッション終了まで
	missionTimer_ -= delta_time;

	//一定時間経ったら
	if (missionTimer_ <= 0) {

		delayTimer_ -= delta_time;

		if (delayTimer_ <= 0) {
			world_->gameData()->setMissionClear(3);

			delayTimer_ = assignmentdelayTimer_;
			world_->gameData()->setBossMake(true);
			float magnification;
			if (missionKillCounter_ <= 10) {
				magnification = 1.5f;
			}
			else if (missionKillCounter_ <= 20) {
				magnification = 2;
			}
			else if (missionKillCounter_ <= 30) {
				magnification = 2.5f;
			}
			else if (missionKillCounter_ <= 40) {
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