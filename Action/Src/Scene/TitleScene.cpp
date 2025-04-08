#include "TitleScene.h"
#include "Common/Assets.h"
#include "cmath"

//開始
void TitleScene::start() {

	//タイトル画面の画像取得
	gsLoadTexture(Texture_Title, "Assets/Texture/title.png");

	//終了フラグの初期化
	is_end_ = false;

	//ロード中のテキスト
	gsLoadTexture(Texture_Load, "Assets/Texture/nowloading2.png");
}

//更新
void TitleScene::update(float delta_time) {

	delta_timer += delta_time;

	if (load_.is_end_) {
		is_end_ = true;
	}

	//エンターキー押したらシーン終了
	if (gsGetKeyTrigger(GKEY_RETURN) && !is_load_) {
		load_.start();
		is_load_ = true;
	}
}

//描画
void TitleScene::draw()const {

	gsDrawSprite2D(Texture_Title, &texturePos_, &textureRect_, NULL, &textureColor_, &textureScale_, 0.0f);

	if (is_load_) {
		CLAMP(Alpha, 0.0f, 1.0f);

		Alpha = (sin((DEG_TO_RAD(delta_timer)) * 2 * M_PI) + 1.0f) / 2.0f;

		color_T.a = Alpha;

		//テキスト
		gsDrawSprite2D(Texture_Load, &pos_T, &rect_T, NULL, &color_T, &scal_T, NULL);
	}
	gsFontParameter(0, 16, "ＭＳ ゴシック");
}

//終了しているか？
bool TitleScene::is_end()const {
	return is_end_; //シーン終了
}

//次のシーン名を返す
std::string TitleScene::next()const {
	return "GamePlayScene"; //次のシーン名を返す
}

//終了
void TitleScene::end() {
	load_.is_end_ = false;
	is_load_ = false;	
	gsDeleteTexture(Texture_Title);
}

void TitleScene::draw_background(GSuint id) const {
}