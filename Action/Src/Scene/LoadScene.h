#ifndef LOAD_GAME_PLAY_SCENE_H_
#define LOAD_GAME_PLAY_SCENE_H_

#include "IScene.h"

// ロード画面（ゲームプレイシーンで使う素材をロードする）
class LoadScene {
public:
	// 開始
	void start();

	// 終了
	void end();

	// 素材の読み込み
	void load();
	// 終了フラグ
	bool is_end_{false};
};
#endif