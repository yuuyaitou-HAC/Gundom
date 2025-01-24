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
	//背景描画
	static const GSvector2 Textureposition{ 532.5,0 };
	static const GSrect TextureRect{ 0,0,855,1078 };
	static const GSvector2 TextureScale{ 1,1 };
	static const GScolor4 textureColor{ 256,256,256,1.0f };
	gsDrawSprite2D(Texture_ResultBuck, &Textureposition, &TextureRect, NULL, &textureColor, &TextureScale, 0.0f);

	gsTextPos(900, 100);
	if (!world_->gameData()->playerDie() && world_->gameData()->missionClear() >= 4) {
		gsDrawText("任務成功");
		ResultScore += 500;
	}
	else {
		gsDrawText("任務失敗");
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
	gsDrawText("バズーカKILL　　　:%d", world_->gameData()->BazookaKillCounter());
	gsTextPos(x, 700);
	gsDrawText("AllRangeUnitKILL  :%d", world_->gameData()->AllRangeUnitKillCounter());

	ResultScore += world_->gameData()->beamRifleKillCounter() +
		world_->gameData()->beamMagnumKillCounter() * 5 +
		world_->gameData()->BazookaKillCounter() * 5 +
		world_->gameData()->AllRangeUnitKillCounter();

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