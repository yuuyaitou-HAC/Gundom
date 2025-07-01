#include <GSgame.h>
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Scene/GamePlayScene.h"
#include "Scene/Screen.h"
#include <GSeffect.h>

//ゲームクラス
class MyGame :public gslib::Game {

public:

	MyGame() : gslib::Game{
		(int)screenwidtht,(int)screenheight,
#if _DEBUG
		false
#else 
		true

#endif
	}{}

	//シーンマネージャー
	SceneManager scene_manager_;

	//開始
	void start()override {
		// エフェクトの初期化
		//gsInitEffect();
		gsInitEffectEx(2048, 4096, 4096, 32);		
		//タイトルシーンの追加
		scene_manager_.add("TitleScene", new TitleScene());
		//ゲームプレイシーンの追加
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		//タイトルシーンから開始
		scene_manager_.change("TitleScene");
		gsHideMouseCursor();
	}
	//更新
	void update(float delta_time)override {
		//シーンの更新
		scene_manager_.update(delta_time);
	}
	//描画
	void draw()override {
		//シーンの描画
		scene_manager_.draw();
	}
	//終了
	void end()override {
		//シーンの終了
		scene_manager_.end();
		// エフェクトの終了
		gsFinishEffect();
	}
};

//メイン関数
int main() {
	return MyGame().run();
}