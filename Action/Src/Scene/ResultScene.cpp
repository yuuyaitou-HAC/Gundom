#include "ResultScene.h"
#include "World\IWorld.h"
#include "Common\Assets.h"
#include "Scene\Screen.h"
#include <gslib.h>
#include <fstream> // ファイル操作に必要
#include "GamePlayScene.h"

ResultScene::ResultScene(World* world) {
	world_ = world;
}

void ResultScene::update(float delta_time) {

}

void ResultScene::draw() const {

	gsTextPos(500, 600);
	gsDrawText("ゲーム終了");

}
