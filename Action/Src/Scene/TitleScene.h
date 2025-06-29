#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include"IScene.h"
#include <gslib.h>
#include "LoadScene.h"

//タイトルシーン
class TitleScene :public IScene {
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

	LoadScene load_;

	bool is_load_;

	//終了フラグ
	bool is_end_{ false };

	GSfloat delta_timer;

	//エンターキー押す
	GSvector2 posEnter_{ 1110.0f,910.0f };
	GSrect rectEnter_{ 0,0,800,200 };
	GScolor colorEnter_{ 1.0f,1.0f,1.0f,1.0f };
	GSvector2 scalEnter_{ 1.0f,1.0f };
	GSfloat AlphaEnter_{ 0.0f };

	//ロード中の画面
	GSvector2 posRoad_{ 1110.0f,910.0f };
	GSrect rectRoad_{ 0,0,800,200 };
	GScolor colorRoad_{ 1.0f,1.0f,1.0f,1.0f };
	GSvector2 scalRoad_{ 1.0f,1.0f };
	GSfloat AlphaRoad_{ 0.0f };

	//タイトル画面
	GSvector2 titleTexturePos_{ 0,0 };
	GSrect titleTextureRect_{ 0,0,1920,1080 };
	GSvector2 titleTextureScale_{ 1,1 };
	GScolor4 titleTextureColor_{ 1,1,1,1 };
};
#endif // !TITLE_SCENE_H_