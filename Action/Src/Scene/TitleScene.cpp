#include "TitleScene.h"
#include "Common/Assets.h"
#include "GSmath.h"

//開始
void TitleScene::start() {

	//タイトル画面の画像取得
	gsLoadTexture(Texture_Title, "Assets/Texture/title.png");

	//終了フラグの初期化
	is_end_ = false;

	//ロード中のテキスト
	gsLoadTexture(Texture_Load, "Assets/Texture/nowloading.png");
	gsLoadTexture(Texture_Enter, "Assets/Texture/EnterKey.png");
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

	//テキストのα値を変える
	if (!is_load_) {
		CLAMP(AlphaEnter_, 0.0f, 1.0f);
		AlphaEnter_ = (sin((DEG_TO_RAD(delta_timer)) * 2 * GS_PI) + 1.0f) / 2.0f;
		colorEnter_.a = AlphaEnter_;
	}
	else {
		CLAMP(AlphaRoad_, 0.0f, 1.0f);
		AlphaRoad_ = (sin((DEG_TO_RAD(delta_timer)) * 2 * GS_PI) + 1.0f) / 2.0f;
		colorRoad_.a = AlphaRoad_;
	}
}

//描画
void TitleScene::draw()const {

	gsDrawSprite2D(Texture_Title, &titleTexturePos_, &titleTextureRect_, NULL, &titleTextureColor_, &titleTextureScale_, 0.0f);

	if (!is_load_) {
		gsDrawSprite2D(Texture_Enter, &posEnter_, &rectEnter_, NULL, &colorEnter_, &scalEnter_, NULL);
	}
	else {
		gsDrawSprite2D(Texture_Load, &posRoad_, &rectRoad_, NULL, &colorRoad_, &scalRoad_, NULL);
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
	gsDeleteTexture(Texture_Load);
}