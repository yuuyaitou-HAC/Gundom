#pragma once

#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include "IScene.h"
#include "World/World.h"
#include "ResultScene.h"

//ゲームプレイシーン
class GamePlayScene :public IScene {
public:

	enum class State {
		Dounyu,
		GameScene,
		OptionScene,
		ResultScene,
	};

public:

	//開始
	virtual void start()override;
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//終了しているか？
	virtual bool is_end()const override;
	//次のシーンを返す
	virtual std::string next()const override;
	//終了
	virtual void end()override;

private:
	void updateDounyuScene(float delta_time);
	void updateGameScene(float delta_time);
	void updateOptionScene(float delta_time);
	void updateResultScene(float delta_time);

private:
	//ワールドクラス
	World world_;

	//シーンの状態
	State state_;

	ResultScene* result_;

private:

	//マニュアル画像表示に使う
	int manualCount;

	int dounyuCount;

	//終了フラグ
	bool is_end_{ false };

	bool pauseFrag;

};

#endif // !GAME_PLAY_SCENE_H_