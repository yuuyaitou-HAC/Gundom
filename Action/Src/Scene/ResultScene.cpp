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
	gsDrawSprite2D(Texture_ResultBuck, &resultBasePos_, &resultBaseRect_, NULL, &textureColor_, &resultBaseScale_, 0.0f);


	//ゲーム結果
	if (!world_->gameData()->playerDie() && world_->gameData()->missionClear() >= 4) {
		gsDrawSprite2D(Texture_GameResult, &gameRisultPos_, &gameRisult1Rect_, NULL, &textureColor_, &gameRisultScale_, 0.0f);
		ResultScore += 500;
	}
	else {
		gsDrawSprite2D(Texture_GameResult, &gameRisultPos_, &gameRisult2Rect_, NULL, &textureColor_, &gameRisultScale_, 0.0f);
	}

	//ミッションのクリア状況
	int mission = world_->gameData()->missionClear();
	switch (mission)
	{
	case 0:
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult1Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult2Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult3Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult4Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		break;
	case 1:
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult1Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult2Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult3Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult4Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		break;
	case 2:
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult1Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult2Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult3Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult4Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		break;
	case 3:
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult1Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult2Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult3Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult4Pos_, &MissionResult2Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		break;
	case 4:
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult1Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult2Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult3Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		gsDrawSprite2D(Texture_MissionRisult, &MissionResult4Pos_, &MissionResult1Rect_, NULL, &textureColor_, &MissionResultScale_, 0.0f);
		break;
	}

	ResultScore += mission * 200;

	//ビームライフルキル
	tens = numRect_[world_->gameData()->beamRifleKillCounter() / 10];
	ichi = numRect_[world_->gameData()->beamRifleKillCounter() % 10];
	numPos_ = { 1090,470 };
	gsDrawSprite2D(Texture_Number2, &numPos_, &tens, NULL, &textureColor_, &numScale_, 0.0f); numPos_ = { 1090,480 };
	numPos_ = { 1140,470 };
	gsDrawSprite2D(Texture_Number2, &numPos_, &ichi, NULL, &textureColor_, &numScale_, 0.0f);

	//ビームマグナムキル
	tens = numRect_[world_->gameData()->beamMagnumKillCounter() / 10];
	ichi = numRect_[world_->gameData()->beamMagnumKillCounter() % 10];
	numPos_ = { 1090,540 };
	gsDrawSprite2D(Texture_Number2, &numPos_, &tens, NULL, &textureColor_, &numScale_, 0.0f); numPos_ = { 1090,480 };
	numPos_ = { 1140,540 };
	gsDrawSprite2D(Texture_Number2, &numPos_, &ichi, NULL, &textureColor_, &numScale_, 0.0f);

	//バズーカキル
	tens = numRect_[world_->gameData()->bazookaKillCounter() / 10];
	ichi = numRect_[world_->gameData()->bazookaKillCounter() % 10];
	numPos_ = { 1090,610 };
	gsDrawSprite2D(Texture_Number2, &numPos_, &tens, NULL, &textureColor_, &numScale_, 0.0f); numPos_ = { 1090,480 };
	numPos_ = { 1140,610 };
	gsDrawSprite2D(Texture_Number2, &numPos_, &ichi, NULL, &textureColor_, &numScale_, 0.0f);

	//ファンネルキル
	tens = numRect_[world_->gameData()->allRangeUnitKillCounter() / 10];
	ichi = numRect_[world_->gameData()->allRangeUnitKillCounter() % 10];
	numPos_ = { 1090,680 };
	gsDrawSprite2D(Texture_Number2, &numPos_, &tens, NULL, &textureColor_, &numScale_, 0.0f); numPos_ = { 1090,480 };
	numPos_ = { 1140,680 };
	gsDrawSprite2D(Texture_Number2, &numPos_, &ichi, NULL, &textureColor_, &numScale_, 0.0f);

	//武器のキルスコア
	ResultScore += world_->gameData()->beamRifleKillCounter() * 10 +
		world_->gameData()->beamMagnumKillCounter() * 5 +
		world_->gameData()->bazookaKillCounter() * 2 +
		world_->gameData()->allRangeUnitKillCounter() * 2;


	int texturenum;
	if (ResultScore < 500)texturenum = 0;
	else if (ResultScore < 1000) texturenum = 1;
	else if (ResultScore < 2000) texturenum = 2;
	else if (ResultScore < 3000) texturenum = 3;
	else texturenum = 4;

	gsDrawSprite2D(Texture_Evaluation, &evaluationPos_, &evaluationRect_[texturenum], NULL, &textureColor_, &evaluationScale_, 0.0f);


}