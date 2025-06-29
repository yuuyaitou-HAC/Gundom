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

	GSuint effect_;

	GSvector3 effectpos_{ GSvector3().zero() };

	//マニュアル画像表示に使う
	int manualCount_{ 0 };

	//導入画面表示
	int dounyuCount_{ 0 };

	//終了フラグ
	bool isEnd_{ false };

	//オプション誘導
	const GSvector2 optionPos_{ 1600,55 };
	const GSrect optionRect_{ 0,0,600,600 };
	const GSvector2 optionScale_{ 0.3f,0.3f };
	const GScolor4 optionColor_{ 256,256,256,1.0f };

	//マニュアル　導入
	const GSvector2 TexturePos_{ 0,0 };
	const GSrect TextureRect_{ 0,0,1920,1080 };
	const GSvector2 TextureScale_{ 1,1 };
	const GScolor4 textureColor_{ 256,256,256,1.0f };
};

#endif // !GAME_PLAY_SCENE_H_