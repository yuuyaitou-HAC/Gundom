#include "ResultScene.h"
#include "World\IWorld.h"
#include "Common\Assets.h"
#include "Scene\Screen.h"
#include <gslib.h>
#include <fstream> // ファイル操作に必要
#include "GamePlayScene.h"

const int x = 600;

ResultScene::ResultScene(World* world) {
	world_ = world;
}

void ResultScene::update(float delta_time) {

}

void ResultScene::draw() const {

	ResultScore = 0;

	//背景描画
	gsDrawSprite2D(Texture_ResultBuck, &resultBasePos_, &resultBaseRect_, NULL, &resultBaseColor_, &resultBaseScale_, 0.0f);


	//ゲーム結果
	if (!world_->gameData()->playerDie() && world_->gameData()->missionClear() >= 4) {
		gsDrawSprite2D(Texture_GameResult, &gameRisultPos_, &gameRisult1Rect_, NULL, &gameRisultColor_, &gameRisultScale_, 0.0f);
	}
	else {
		gsDrawSprite2D(Texture_GameResult, &gameRisultPos_, &gameRisult2Rect_, NULL, &gameRisultColor_, &gameRisultScale_, 0.0f);
	}

	//ミッションのクリア状況
	int mission = world_->gameData()->missionClear();
	switch (mission)
	{
	case 0:
		gsTextPos(x, 300);
		gsDrawText("ミッション1未達成");
		gsTextPos(x, 350);
		gsDrawText("ミッション2未達成");
		gsTextPos(x, 400);
		gsDrawText("ミッション3未達成");
		gsTextPos(x, 450);
		gsDrawText("ミッション4未達成");
		break;
	case 1:
		gsTextPos(x, 300);
		gsDrawText("ミッション1達成");
		gsTextPos(x, 350);
		gsDrawText("ミッション2未達成");
		gsTextPos(x, 400);
		gsDrawText("ミッション3未達成");
		gsTextPos(x, 450);
		gsDrawText("ミッション4未達成");
		break;
	case 2:
		gsTextPos(x, 300);
		gsDrawText("ミッション1達成");
		gsTextPos(x, 350);
		gsDrawText("ミッション2達成");
		gsTextPos(x, 400);
		gsDrawText("ミッション3未達成");
		gsTextPos(x, 450);
		gsDrawText("ミッション4未達成");
		break;
	case 3:
		gsTextPos(x, 300);
		gsDrawText("ミッション1達成");
		gsTextPos(x, 350);
		gsDrawText("ミッション2達成");
		gsTextPos(x, 400);
		gsDrawText("ミッション3達成");
		gsTextPos(x, 450);
		gsDrawText("ミッション4未達成");
		break;
	case 4:
		gsTextPos(x, 300);
		gsDrawText("ミッション1達成");
		gsTextPos(x, 350);
		gsDrawText("ミッション2達成");
		gsTextPos(x, 400);
		gsDrawText("ミッション3達成");
		gsTextPos(x, 450);
		gsDrawText("ミッション4達成");
		break;
	}

	ResultScore += mission * 50;

	gsTextPos(x, 550);
	gsDrawText("ビームライフルKILL:%d", world_->gameData()->beamRifleKillCounter());
	gsTextPos(x, 600);
	gsDrawText("ビームマグナムKILL:%d", world_->gameData()->beamMagnumKillCounter());
	gsTextPos(x, 650);
	gsDrawText("バズーカKILL　　　:%d", world_->gameData()->bazookaKillCounter());
	gsTextPos(x, 700);
	gsDrawText("AllRangeUnitKILL  :%d", world_->gameData()->allRangeUnitKillCounter());

	ResultScore += world_->gameData()->beamRifleKillCounter() +
		world_->gameData()->beamMagnumKillCounter() * 5 +
		world_->gameData()->bazookaKillCounter() * 5 +
		world_->gameData()->allRangeUnitKillCounter();

	gsTextPos(900, 800);
	if (ResultScore < 500) {
		gsDrawText("D");
	}
	else if (ResultScore < 1000) {
		gsDrawText("C");
	}
	else if (ResultScore < 2000) {
		gsDrawText("B");
	}
	else if (ResultScore < 3000) {
		gsDrawText("A");
	}
	else {
		gsDrawText("S");
	}
}