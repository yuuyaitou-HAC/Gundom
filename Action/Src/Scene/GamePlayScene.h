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
		GameScene,
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

	void updateGameScene(float delta_time);
	void updateResultScene(float delta_time);

private:
	//ワールドクラス
	World world_;

	//シーンの状態
	State state_;

	ResultScene* result_;

private:
	//終了フラグ
	bool is_end_{ false };
};

#endif // !GAME_PLAY_SCENE_H_