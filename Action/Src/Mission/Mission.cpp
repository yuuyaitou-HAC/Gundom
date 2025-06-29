#include "Mission.h"
#include "World/IWorld.h"
#include "Player/Player.h"
#include "UnderBoss/UnderBoss.h"
#include "Common/Assets.h"

#include "imgui/imgui.h"



Mission::Mission(IWorld* world, const GSvector3& position) :
	state_{ State::Mission3 } {

	world_ = world;

	tag_ = "MissionTag";
	name_ = "Mission";

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));
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
		if (!world_->gameData()->underBossDie() && underBoss_ != NULL) {
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
		if (!world_->gameData()->bossDie() && boss_ != NULL) {
			float maxhp = boss_->boss_state()->MaxHP();
			float hp = boss_->boss_state()->HP();
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

		if (world_->gameData()->dieEnemyCounter() < enemyDieCounter_) {

			//ミッション内容
			gsDrawSprite2D(Texture_Mission1, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);

			//撃破数
			gsDrawSprite2D(Texture_KillNum, &killnumPosition_, &killnumRect_, NULL, &textureColor_, &killnumScale_, 0.0f);

			//現在の撃破数
			if (world_->gameData()->dieEnemyCounter() >= 10) {
				tens = numRect_[world_->gameData()->dieEnemyCounter() / 10];
				ones = numRect_[world_->gameData()->dieEnemyCounter() % 10];

				gsDrawSprite2D(Texture_Number, &nowKillCountTen_, &tens, NULL, &textureColor_, &numScale_, 0.0f);
				gsDrawSprite2D(Texture_Number, &nowKillCountOne_, &ones, NULL, &textureColor_, &numScale_, 0.0f);

			}
			else {
				ones = numRect_[world_->gameData()->dieEnemyCounter()];
				gsDrawSprite2D(Texture_Number, &nowKillCountOne_, &ones, NULL, &textureColor_, &numScale_, 0.0f);
			}

			//スラッシュ
			gsDrawSprite2D(Texture_Slash, &slashPosition_, &slashRect_, NULL, &textureColor_, &slashScale_, 0.0f);

			//目標撃破数
			if (enemyDieCounter_ >= 10) {
				tens = numRect_[enemyDieCounter_ / 10];
				ones = numRect_[enemyDieCounter_ % 10];

				gsDrawSprite2D(Texture_Number, &objectiveKillCountTen_, &tens, NULL, &textureColor_, &numScale_, 0.0f);
				gsDrawSprite2D(Texture_Number, &objectiveKillCountOne_, &ones, NULL, &textureColor_, &numScale_, 0.0f);

			}
			else {
				ones = numRect_[enemyDieCounter_];
				gsDrawSprite2D(Texture_Number, &objectiveKillCountTen_, &ones, NULL, &textureColor_, &numScale_, 0.0f);
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

			if (underBoss_ != NULL) {

				//体力バー
				//HPバー(青)
				gsDrawSprite2D(Texture_HPBer, &hpposition_, &hpRect_,
					NULL, &textureColor_, &hpScale_, 0.0f);

				GSvector2 HPBackScale{ hpBarScale_,2.5 };
				gsDrawSprite2D(Texture_HPBack, &hpBackposition_, &hpBackRect_,
					NULL, &textureColor_, &HPBackScale, 180.0f);

				//現在の中ボスのHP
				hundreds = numRect_[underBoss_->underBossState_()->HP() / 100];
				tens = numRect_[(underBoss_->underBossState_()->HP() / 10) % 10];
				ones = numRect_[underBoss_->underBossState_()->HP() % 10];
				gsDrawSprite2D(Texture_Number, &nowUndreBossHPHundred_, &hundreds, NULL, &textureColor_, &numScaleHP_, 0.0f);
				gsDrawSprite2D(Texture_Number, &nowUndreBossHPTen_, &tens, NULL, &textureColor_, &numScaleHP_, 0.0f);
				gsDrawSprite2D(Texture_Number, &nowUndreBossHPOne_, &ones, NULL, &textureColor_, &numScaleHP_, 0.0f);

				//スラッシュ
				gsDrawSprite2D(Texture_Slash, &slashPositionHP_, &slashRectHP_, NULL, &textureColor_, &slashScaleHP_, 0.0f);

				//マックス時の中ボスのHP
				hundreds = numRect_[underBoss_->underBossState_()->MaxHP() / 100];
				tens = numRect_[(underBoss_->underBossState_()->MaxHP() / 10) % 10];
				ones = numRect_[underBoss_->underBossState_()->MaxHP() % 10];
				gsDrawSprite2D(Texture_Number, &maxUndreBossHPHundred_, &hundreds, NULL, &textureColor_, &numScaleHP_, 0.0f);
				gsDrawSprite2D(Texture_Number, &maxUndreBossHPTen_, &tens, NULL, &textureColor_, &numScaleHP_, 0.0f);
				gsDrawSprite2D(Texture_Number, &maxUndreBossHPOne_, &ones, NULL, &textureColor_, &numScaleHP_, 0.0f);

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
		if (mission3Timer_ > 0) {

			//ミッション内容
			gsDrawSprite2D(Texture_Mission3, &missionPosition_, &missionRect_, NULL, &textureColor_, &missionScale_, 0.0f);

			//分
			thousand = numRect_[(int)(mission3Timer_ / 3600) / 10];
			hundreds = numRect_[(int)(mission3Timer_ / 3600) % 10];
			//秒
			tens = numRect_[(((int)mission3Timer_ % 3600) / 60) / 10];
			ones = numRect_[(((int)mission3Timer_ % 3600) / 60) % 10];

			//ミッション時間
			gsDrawSprite2D(Texture_Number, &nowMissionTimerthousand_, &thousand, NULL, &textureColor_, &mission3NumScale_, 0.0f);
			gsDrawSprite2D(Texture_Number, &nowMissionTimerhundred_, &hundreds, NULL, &textureColor_, &mission3NumScale_, 0.0f);
			gsDrawSprite2D(Texture_Clon, &nowMissionTimerClon_, &numRect_[0], NULL, &textureColor_, &mission3NumScale_, 0.0f);
			gsDrawSprite2D(Texture_Number, &nowMissionTimerten_, &tens, NULL, &textureColor_, &mission3NumScale_, 0.0f);
			gsDrawSprite2D(Texture_Number, &nowMissionTimerone_, &ones, NULL, &textureColor_, &mission3NumScale_, 0.0f);

			gsDrawSprite2D(Texture_MissionTimer, &mtPosition_, &mtRect_, NULL, &textureColor_, &mtScale_, 0.0f);

			//ミッション中の殲滅数
			tens = numRect_[missionKillCounter_ / 10];
			ones = numRect_[missionKillCounter_ % 10];

			gsDrawSprite2D(Texture_Number, &mission3KillCountTen_, &tens, NULL, &textureColor_, &mission3NumScale_, 0.0f);
			gsDrawSprite2D(Texture_Number, &mission3KillCountOne_, &ones, NULL, &textureColor_, &mission3NumScale_, 0.0f);
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
			gsDrawSprite2D(Texture_HPBer, &hpposition_, &hpRect_, NULL, &textureColor_, &hpScale_, 0.0f);

			GSvector2 HPBackScale{ hpBarScale_,2.5 };
			gsDrawSprite2D(Texture_HPBack, &hpBackposition_, &hpBackRect_, NULL, &textureColor_, &HPBackScale, 180.0f);

			//現在のHP
			thousand = numRect_[boss_->boss_state()->HP() / 1000];
			hundreds = numRect_[(boss_->boss_state()->HP() / 100) % 10];
			tens = numRect_[(boss_->boss_state()->HP() / 10) % 10];
			ones = numRect_[boss_->boss_state()->HP() % 10];

			gsDrawSprite2D(Texture_Number, &nowBossHPthousand_, &thousand, NULL, &textureColor_, &numScaleHP_, 0.0f);
			gsDrawSprite2D(Texture_Number, &nowBossHPHundred_, &hundreds, NULL, &textureColor_, &numScaleHP_, 0.0f);
			gsDrawSprite2D(Texture_Number, &nowBossHPTen_, &tens, NULL, &textureColor_, &numScaleHP_, 0.0f);
			gsDrawSprite2D(Texture_Number, &nowBossHPOne_, &ones, NULL, &textureColor_, &numScaleHP_, 0.0f);

			//スラッシュ
			gsDrawSprite2D(Texture_Slash, &slashPositionHP_, &slashRectHP_, NULL, &textureColor_, &slashScaleHP_, 0.0f);

			//マックス時のボスのHP
			thousand = numRect_[boss_->boss_state()->MaxHP() / 1000];
			hundreds = numRect_[(boss_->boss_state()->MaxHP() / 100) % 10];
			tens = numRect_[(boss_->boss_state()->MaxHP() / 10) % 10];
			ones = numRect_[boss_->boss_state()->MaxHP() % 10];

			gsDrawSprite2D(Texture_Number, &maxBossHPthousand_, &thousand, NULL, &textureColor_, &numScaleHP_, 0.0f);
			gsDrawSprite2D(Texture_Number, &maxBossHPHundred_, &hundreds, NULL, &textureColor_, &numScaleHP_, 0.0f);
			gsDrawSprite2D(Texture_Number, &maxBossHPTen_, &tens, NULL, &textureColor_, &numScaleHP_, 0.0f);
			gsDrawSprite2D(Texture_Number, &maxBossHPOne_, &ones, NULL, &textureColor_, &numScaleHP_, 0.0f);

			//ボスのHP説明
			gsDrawSprite2D(Texture_BossHP, &ubHPPosition_, &ubHPRect_, NULL, &textureColor_, &ubHPScale_, 0.0f);
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
	if (world_->gameData()->dieEnemyCounter() >= enemyDieCounter_) {

		delayTimer_ -= delta_time;

		if (delayTimer_ <= 0) {
			//ゲームデータのミッションクリア状況更新
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
			//ゲームデータのミッションクリア状況更新
			world_->gameData()->setMissionClear(2);
			delayTimer_ = assignmentdelayTimer_;
			//今までの退却させた部隊数
			beforKillCounter_ = world_->gameData()->dieEnemyCounter();
			state_ = State::Mission3;
		}
	}
}

void Mission::mission3(float delta_time) {

	//ミッション終了まで
	mission3Timer_ -= delta_time;

	//一定時間経ったら
	if (mission3Timer_ <= 0) {

		delayTimer_ -= delta_time;

		if (delayTimer_ <= 0) {
			//ゲームデータのミッションクリア状況更新
			world_->gameData()->setMissionClear(3);

			delayTimer_ = assignmentdelayTimer_;
			world_->gameData()->setBossMake(true);

			//撃破数に応じてプレイヤーにバフ付与
			float magnification;
			if (missionKillCounter_ <= missionBuffKILL_[0]) {
				magnification = missionBuffMagnification_[0];
			}
			else if (missionKillCounter_ <= missionBuffKILL_[1]) {
				magnification = missionBuffMagnification_[1];
			}
			else if (missionKillCounter_ <= missionBuffKILL_[2]) {
				magnification = missionBuffMagnification_[2];
			}
			else if (missionKillCounter_ <= missionBuffKILL_[3]) {
				magnification = missionBuffMagnification_[3];
			}
			else {
				magnification = missionBuffMagnification_[4];
			}
			playerstateup(magnification);
			state_ = State::Mission4;
		}
	}
}

void Mission::mission4(float delta_time) {

	if (world_->gameData()->bossDie()) {

		//ゲームデータのミッションクリア状況更新
		world_->gameData()->setMissionClear(4);

		state_ = State::GameClear;
	}
}

void Mission::gameClear(float delta_time) {

}

//ミッションの成功報酬
void Mission::playerstateup(float magnification) {
	//maxhp
	player_->player_state()->addMaxHP(player_->player_state()->maxHP() * magnification);

	//HP
	player_->player_state()->AddHP(player_->player_state()->hp() * magnification);

	//攻撃
	player_->player_state()->addAttack(player_->player_state()->attack() * magnification);

	//防御
	player_->player_state()->addDefense(player_->player_state()->defense() * magnification);

	//スラスター
	player_->player_state()->addMaxEnargy(player_->player_state()->MaxEnargy() * magnification);
	player_->player_state()->addEnargy(player_->player_state()->MaxEnargy());
}