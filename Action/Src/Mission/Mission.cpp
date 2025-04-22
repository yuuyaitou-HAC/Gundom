#include "Mission.h"
#include "World/IWorld.h"
#include "Player/Player.h"
#include "UnderBoss/UnderBoss.h"
#include "Common/Assets.h"


//ミッション１のノルマ
const int MakeBossCounter_{ 19 };

Mission::Mission(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "MissionTag";
	name_ = "Mission";

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	state_ = State::Mission1;
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
			HPBarScale = (maxhp - hp) / maxhp;
			HPBarScale = CLAMP(HPBarScale, 0, 1);
		}
		mission2(delta_time);
		break;
	case Mission::State::Mission3:
		MissionKillCounter_ = world_->gameData()->dieEnemyCounter() - beforKillCounter_;
		mission3(delta_time);
		break;
	case Mission::State::Mission4:
		if (boss_ != NULL) {
			float maxhp = boss_->bossState_()->MaxHP();
			float hp = boss_->bossState_()->HP();
			HPBarScale = (maxhp - hp) / maxhp;
			HPBarScale = CLAMP(HPBarScale, 0, 1);
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
	static const GSvector2 Textureposition{ 80,50 };
	static const GSrect TextureRect{ 0,0,800,600 };
	static const GSvector2 TextureScale{ 0.5,0.3 };
	static const GScolor4 textureColor{ 256,256,256,1.0f };
	gsDrawSprite2D(Texture_MissionBack, &Textureposition, &TextureRect, NULL, &textureColor, &TextureScale, 0.0f);



	switch (state_)
	{
	case Mission::State::Mission1:

		if (world_->gameData()->dieEnemyCounter() < MakeBossCounter_) {

			//ミッション内容
			gsDrawSprite2D(Texture_Mission1, &missionPosition_, &missionRect_, NULL, &TextureColor, &missionScale_, 0.0f);

			//撃破数
			gsDrawSprite2D(Texture_KillNum, &killnumPosition_, &killnumRect_, NULL, &TextureColor, &killnumScale_, 0.0f);

			//現在の撃破数
			if (world_->gameData()->dieEnemyCounter() >= 10) {
				GSrect juu = NumRect[world_->gameData()->dieEnemyCounter() / 10];
				GSrect ichi = NumRect[world_->gameData()->dieEnemyCounter() % 10];

				numpos = GSvector2{ 850,100 };
				gsDrawSprite2D(Texture_Number, &numpos, &juu, NULL, &TextureColor, &numScale_, 0.0f);
				numpos = GSvector2{ 900,100 };
				gsDrawSprite2D(Texture_Number, &numpos, &ichi, NULL, &TextureColor, &numScale_, 0.0f);

			}
			else {
				GSrect ichi = NumRect[world_->gameData()->dieEnemyCounter()];
				numpos = GSvector2{ 900,100 };
				gsDrawSprite2D(Texture_Number, &numpos, &ichi, NULL, &TextureColor, &numScale_, 0.0f);
			}

			gsDrawSprite2D(Texture_Slash, &slashPosition_, &slashRect_, NULL, &TextureColor, &slashScale_, 0.0f);

			//目標撃破数
			if (MakeBossCounter_ >= 10) {
				GSrect juu = NumRect[MakeBossCounter_ / 10];
				GSrect ichi = NumRect[MakeBossCounter_ % 10];

				numpos = GSvector2{1000,100};
				gsDrawSprite2D(Texture_Number, &numpos, &juu, NULL, &TextureColor, &numScale_, 0.0f);
				numpos = GSvector2{ 1050,100 };
				gsDrawSprite2D(Texture_Number, &numpos, &ichi, NULL, &TextureColor, &numScale_, 0.0f);

			}
			else {
				GSrect ichi = NumRect[MakeBossCounter_];
				numpos = GSvector2{ 1000,100 };
				gsDrawSprite2D(Texture_Number, &numpos, &ichi, NULL, &TextureColor, &numScale_, 0.0f);
			}

			
		}
		else {
			gsTextPos(800, 100);
			gsDrawText("中BOSSが出現した");
		}
		break;

	case Mission::State::Mission2:

		if (world_->gameData()->underBossDie() == false) {

			//ミッション内容
			gsDrawSprite2D(Texture_Mission1, &missionPosition_, &missionRect_, NULL, &TextureColor, &missionScale_, 0.0f);

			gsTextPos(800, 150);

			if (underBoss_ != NULL) {
				gsDrawText("中BOSSのHP:%d/%d", underBoss_->underBossState_()->HP(), underBoss_->underBossState_()->MaxHP());


				//体力バー
				//HPバー(青)
				gsDrawSprite2D(Texture_HP, &HPposition, &HPRect,
					NULL, &TextureColor, &HPScale, 0.0f);

				GSvector2 HPBackScale{ HPBarScale,1 };
				gsDrawSprite2D(Texture_HPBack, &HPBackposition, &HPBackRect,
					NULL, &TextureColor, &HPBackScale, 180.0f);
			}
		}
		if (world_->gameData()->underBossDie() == true) {
			gsTextPos(800, 100);
			gsDrawText("中BOSSを撃破した!!");
			gsTextPos(800, 120);
			gsDrawText("このエリアを制圧するため、より多くの敵部隊を壊滅させよ");
		}
		break;

	case Mission::State::Mission3:
		if (MissionTimer > 0) {
			gsTextPos(100, 100);
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
		gsTextPos(100, 100);
		gsDrawText("Mission2：BOSSを倒せ");
		gsTextPos(800, 150);
		if (boss_ != NULL) {
			gsDrawText("BOSSのHP:%d/%d", boss_->bossState_()->HP(), boss_->bossState_()->MaxHP());

			//体力バー
				//HPバー(青)
			gsDrawSprite2D(Texture_HP, &HPposition, &HPRect,
				NULL, &TextureColor, &HPScale, 0.0f);

			GSvector2 HPBackScale{ HPBarScale,1 };
			gsDrawSprite2D(Texture_HPBack, &HPBackposition, &HPBackRect,
				NULL, &TextureColor, &HPBackScale, 180.0f);

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
			world_->gameData()->setUnderBossMake(true);
			state_ = State::Mission2;
		}
	}
}

void Mission::mission2(float delta_time) {

	//ボス死んだら
	if (world_->gameData()->underBossDie()) {

		delay_timer -= delta_time;

		if (delay_timer <= 0) {
			world_->gameData()->setMissionClear(2);
			delay_timer = Assignmentdelay_timer;
			//Mission3の時間 7200
			MissionTimer = 3600.0f;
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