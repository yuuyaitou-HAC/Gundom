#ifndef PLAYER_UI_H_
#define PLAYER_UI_H_

#include <gslib.h>
#include "Player/PlayerState.h"

class PlayerUI {

public:

	PlayerUI(PlayerState* ps);

	void update(float delta_time);

	//プレイヤー側で呼ぶ描画処理
	void drawgui() const;

private:

	//HPバーの描画
	void drawHPBer()const;

	//EXスキルバーの描画
	void drawEXBer()const;

	//武器のシルエットの描画
	void drawWeaponSilhouette()const;

	//武器シルエットのサイズ
	void weaponSilhouetteSize();

	//スラスター残量のber
	void drawThrusterBer()const;

	//レティクル描画
	void drawreticle()const;

private:

	PlayerState* playerState_;

private:

	//テクスチャに必要な変数
private:

	//レティクル
	GSrect    reticle_rect{ 0, 0, 32, 32 };
	mutable GSvector2 reticle_position;
	GSvector2 reticle_center{ 16,16 };

	mutable float hpBarScale_;

	//HPバー(青)
	GSvector2 hpBerPosition_{ 180,880 };
	GSrect hpBerRect_{ 0,0,500,40 };
	GSvector2 hpBerScale_{ 1,1 };
	GScolor4 hpBerColor_{ 256,256,256,1.0f };

	//HPバー(灰)
	GSvector2 hpBackPosition_{ 680,920 };
	GSrect hpBackRect_{ 0,0,500,40 };
	GScolor4 hpBackColor_{ 256,256,256,1.0f };

	//必殺ゲージバー
	GSvector2 exBerPosition_{ 180,920 };
	GSrect exBerRect_{ 0,0,500,20 };
	GSvector2 exBerScale_{ 1,1 };
	GScolor4 exBerColor_{ 256,256,256,1.0f };

	//EX表示
	GSvector2 exPosition_{ 115,920 };
	GSrect exRect_{ 0,0,600,300 };
	GSvector2 exScale_{ 0.07,0.07 };
	GScolor4 exColor_{ 256,256,256,1.0f };

	mutable GSvector2 enargyBarScale_;

	//EXボール
	GSrect exBallRect_{ 0,0,40,40 };
	GSvector2 exBallScale_{ 1,1 };
	GScolor4 exBallColor_{ 256,256,256,1.0f };

	//EXボールの各座標
	GSvector2 exBallPosition1_{ 100, 840 };
	GSvector2 exBallPosition2_{ 60, 880 };
	GSvector2 exBallPosition3_{ 100, 920 };

	//スラスター
	GSvector2 thrusterBackPosition_{ 1210,900 };
	GSrect thrusterBackRect_{ 0,0,500,20 };
	GScolor4 thrusterBackColor_{ 256,256,256,1.0f };
	mutable GSvector2 thrusterBackScale_{ 1,1 };

	GSvector2 thrusterPosition_{ 710,880 };
	GSrect thrusterRect_{ 0,0,500,20 };
	GSvector2 thrusterScale_{ 1,1 };
	GScolor4 thrusterColor_{ 256,256,256,1.0f };


	GSvector2 beamLiflePosition_{ 1300,780 };
	GSrect beamLifleRect_{ 0,0,3300,1090 };
	GSvector2 beamLifleScale_{ 0.06,0.06 };
	GSvector2 assignmentBeamLifleScale_{ 0.06,0.06 };
	GScolor4 beamLifleColor_{ 256,256,256,1.0f };

	GSvector2 beamMagnumPosition_{ 1300,850 };
	GSrect beamMagnumRect_{ 0,0,4020,2220 };
	GSvector2 beamMagnumScale_{ 0.03,0.03 };
	GSvector2 assignmentBeamMagnumScale_{ 0.03,0.03 };
	GScolor4 beamMagnumColor_{ 256,256,256,0.5f };

	GSvector2 bazookaPosition_{ 1300,930 };
	GSrect bazookaRect_{ 0,0,2000,660 };
	GSvector2 bazookaScale_{ 0.1,0.1 };
	GSvector2 assignmentBazookaScale_{ 0.1,0.1 };
	GScolor4 bazookaColor_{ 256,256,256,0.5f };

	GSvector2 bulletPosition_{ 1600,780 };
	GSrect bulletRect_{ 0,0,490,1135 };
	GSvector2 bulletScale_{ 0.05,0.05 };
	GScolor4 bulletColor_{ 256,256,256,1.0f };

	GSvector2 magajinPosition_{ 1700,780 };
	GSrect magajinRect_{ 0,0,330,500 };
	GSvector2 magajinScale_{ 0.11,0.11 };
	GScolor4 magajinColor_{ 256,256,256,1.0f };

	GSvector2 hpPosition_{ 115,880 };
	GSrect hpRect_{ 0,0,600,300 };
	GSvector2 hpScale_{ 0.1,0.1 };
	GScolor4 hpColor_{ 256,256,256,1.0f };

	//数値
	mutable GSvector2 numPos_;
	GSvector2 numScale_{ 0.5,0.5 };
	GSrect numRect_[10]{
	GSrect{0,  0, 60,  60},
	GSrect{0, 60, 60, 120},
	GSrect{0,120, 60, 180},
	GSrect{0,180, 60, 240},
	GSrect{0,240, 60, 300},
	GSrect{0,300, 60, 360},
	GSrect{0,360, 60, 420},
	GSrect{0,420, 60, 480},
	GSrect{0,480, 60, 540},
	GSrect{0,540, 60, 600}
	};
	GScolor4 numColor_{ 256,256,256,1.0f };

	//＊
	mutable GSvector2 asteriskPosition_;
	GSrect asteriskRect_{ 0,0,60,60 };
	GSvector2 asteriskScale_{ 0.5,0.5 };
	GScolor4 asteriskColor_{ 256,256,256,1.0f };

	mutable GSrect bulletNum_;

	float magnification_ = 1.2f;

	float soundValue_;
};
#endif // !PLAYER_UI_H_