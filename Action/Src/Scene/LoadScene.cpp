#include "LoadScene.h"
#include <GSgame.h>
#include "Common/Assets.h"
#include <GSstandard_shader.h>
#include "GSeffect.h"

void LoadScene::start() {
	//初期化
	is_end_ = false;
	loaded_count = 0;

	//ロードを別スレッドで開始
	gslib::Game::run_thread([=] {load(); });
}

void LoadScene::draw() const {
	gsTextPos(500, 500);
	gsDrawText("ロード中ロード中ロード中");
}

void LoadScene::end() {

}

void LoadScene::load() {

	//視錐台カリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);
	// デフォルトシェーダーの初期化（メッシュファイルを読み込む前に有効にする）
	gsInitDefaultShader();
	//スカイドーム用のメッシュを読み込む
	gsLoadMeshFixed(Mesh_Skybox, "Assets/Skybox/skydome.msh");
	// プレイヤーメッシュの読み込み
	gsLoadSkinMesh(Mesh_Player, "Assets/Robo2/Player.msh");
	//人型の敵
	gsLoadSkinMesh(Mesh_HBM, "Assets/EnemyBase/EnemyBase.msh");
	//ボス
	gsLoadSkinMesh(Mesh_Boss, "Assets/Boss/Boss.msh");
	//戦車のメッシュ
	gsLoadSkinMesh(Mesh_Enemy, "Assets/sennsya/Sennsya.mshb");
	//武器のメッシュを読み込む
	gsLoadMesh(Mesh_Weapon, "Assets/Weapon/w_magun01.msh");
	//ビームサーベルを読み込む
	gsLoadMesh(Mesh_BeamSbred, "Assets/BeamSabre/BeamSabre.mshb");
	//ビームサーベル展開時
	gsLoadMesh(Mesh_BeamSbred2, "Assets/BeamSabre2/BeamSabre2.mshb");
	//敵戦艦を読み込む
	gsLoadMesh(Mesh_EnemyShip, "Assets/EnemyShip/EnemyShip.mshb");
	//味方戦艦を読み込む
	gsLoadMesh(Mesh_PlayerShip, "Assets/PlayerShip/PlayerShip.mshb");
	//ファンネル
	gsLoadMesh(Mesh_AllRangeUnit, "Assets/AllRangeUnit/AllRangeUnit.mshb");

	//今のステージ
	gsLoadOctree(Octree_Stage2, "Assets/Stage2/Octree/Octree.oct");
	gsLoadOctree(Octree_Collider2, "Assets/Stage2/ColliderMesh/Collider.oct");
	gsLoadLightmap(0, "Assets/Stage2/Lightmap/Lightmap.txt");
	gsLoadReflectionProbe(0, "Assets/Stage2/RefProbe/ReflectionProbe.txt");

	//テクスチャ
	gsLoadTexture(Texture_ResultBuck, "Assets/Texture/Risult.png");
	gsLoadTexture(Texture_EX1, "Assets/Texture/EX1.png");
	gsLoadTexture(Texture_EX2, "Assets/Texture/EX2.png");
	gsLoadTexture(Texture_EX3, "Assets/Texture/EX3.png");
	gsLoadTexture(Texture_EX4, "Assets/Texture/EX4.png");
	gsLoadTexture(Texture_EX1Ball, "Assets/Texture/EXball1.png");
	gsLoadTexture(Texture_EX2Ball, "Assets/Texture/EXball2.png");
	gsLoadTexture(Texture_EX3Ball, "Assets/Texture/EXball3.png");
	gsLoadTexture(Texture_EX4Ball, "Assets/Texture/EXball4.png");
	gsLoadTexture(Texture_Buster1, "Assets/Texture/Buster1.png");
	gsLoadTexture(Texture_Buster2, "Assets/Texture/Buster2.png");
	gsLoadTexture(Texture_HP, "Assets/Texture/HPBar.png");
	gsLoadTexture(Texture_HPBack, "Assets/Texture/HPBar2.png");
	gsLoadTexture(Texture_BeamLifle, "Assets/Texture/beamrifle.png");
	gsLoadTexture(Texture_BeamMagnum, "Assets/Texture/beamMagnam.png");
	gsLoadTexture(Texture_Bazooka, "Assets/Texture/bazooka.png");
	gsLoadTexture(Texture_Bullet, "Assets/Texture/Bullet.png");
	gsLoadTexture(Texture_Magajin, "Assets/Texture/Magazin.png");


	//エフェクト
	gsLoadEffect(Effect_PBeamRifle, "Assets/Effect/Arrow01_Purple.efkefc");
	gsLoadEffect(Effect_PBeamMagnum, "Assets/Effect/Arrow02_Purple.efkefc");
	gsLoadEffect(Effect_vernierBL, "Assets/Effect/Thruster_L_Blue.efkefc");
	gsLoadEffect(Effect_vernierBS, "Assets/Effect/Thruster_S_Blue.efkefc");
	gsLoadEffect(Effect_vernierBSS, "Assets/Effect/Thruster_SS_Blue.efkefc");

	is_end_ = true;
}
