#include "GamePlayScene.h"
#include "ResultScene.h"
#include "Field/Field.h"
#include "Camera/CameraTPS.h"
#include "Rendering/Light.h"
#include "Player/Player.h"
#include "Common/Assets.h"
#include "Common/GameData.h"
#include "Gun/GunControl.h"
#include <GSstandard_shader.h>
#include "BattleShip/EnemyShip.h"
#include "BattleShip/PlayerShip.h"
#include "Mission/Mission.h"


//開始
void GamePlayScene::start() {
	//終了フラグを初期化
	is_end_ = false;

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

	gsLoadMesh(Mesh_AllRangeUnit, "Assets/AllRangeUnit/AllRangeUnit.mshb");

	//今のステージ
	gsLoadOctree(Octree_Stage2, "Assets/Stage2/Octree/Octree.oct");
	gsLoadOctree(Octree_Collider2, "Assets/Stage2/ColliderMesh/Collider.oct");
	gsLoadLightmap(0, "Assets/Stage2/Lightmap/Lightmap.txt");
	gsLoadReflectionProbe(0, "Assets/Stage2/RefProbe/ReflectionProbe.txt");

	gsLoadTexture(Texture_ResultBuck, "Assets/Texture/Risult.png");

	//フィールドの追加
	world_.add_field(new Field{ Octree_Stage2,Octree_Collider2,Mesh_Skybox });

	//カメラ
	world_.add_camera(new CameraTPS{ &world_,GSvector3{204,-8,8.5},GSvector3{0.0f,0.0f,0.0f} });
	//ライトの追加
	world_.add_light(new Light{ &world_ });

	world_.add_gamedata(new GameData{});

	//プレイヤーの追加
	world_.add_actor(new Player{ &world_,GSvector3{204,-8,8.5} });

	//弾管理クラス
	world_.add_actor(new GunControl{ &world_,GSvector3{0.f,0.f,0.f} });

	//戦艦
	world_.add_actor(new EnemyShip{ &world_,GSvector3{-120,10,1.5} });

	//味方戦艦
	world_.add_actor(new PlayerShip{ &world_,GSvector3{231.5,10,8} });

	//ミッションクラス	
	world_.add_actor(new Mission{ &world_,GSvector3{122.2,10,-10} });

	//シャドウマップの作成
	static const GSuint shadow_map_size[] = { 2048,2048 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	//シャドウマップを適応する距離（視点からの距離）
	gsSetShadowMapDistance(60.f);
	//カスケードシャドウマップの分割位置を調整（デフォルトは0.5）
	gsSetShadowMapCascadeLamda(0.7f);
	//シャドウの濃さを設定（0.0:濃い～1.0:薄い）
	gsSetShadowMapAttenuation(0.f);

	//初期化
	world_.gameData()->initialize();

	//ゲームシーン開始
	state_ = State::GameScene;

	result_ = new ResultScene{ &world_ };
}

//更新
void GamePlayScene::update(float delta_time) {

	switch (state_)
	{
	case GamePlayScene::State::GameScene:
		updateGameScene(delta_time);
		break;
	case GamePlayScene::State::ResultScene:
		updateResultScene(delta_time);
		break;
	}

	//リザルト
	if (gsGetKeyTrigger(GKEY_P)) state_ = State::ResultScene;
	//ゲーム終了
	if (gsGetKeyTrigger(GKEY_O)) is_end_ = true;
}

//描画
void GamePlayScene::draw()const {
	//ワールドの描画
	world_.draw();

	//リザルト描画
	if (state_ == State::ResultScene)result_->draw();
}

//終了しているか？
bool GamePlayScene::is_end()const {
	return is_end_; //終了フラグを返す
}

//次のシーンを返す
std::string GamePlayScene::next()const {
	return "TitleScene"; //タイトルシーンに戻る
}

//終了
void GamePlayScene::end() {
	//ワールドのクリア
	world_.clear();

	delete result_;

	// メッシュの削除
	gsDeleteSkinMesh(Mesh_Player);
	gsDeleteSkinMesh(Mesh_Enemy);
	gsDeleteSkinMesh(Mesh_Boss);
	gsDeleteSkinMesh(Mesh_HBM);
	gsDeleteSkinMesh(Mesh_Weapon);
	gsDeleteSkinMesh(Mesh_BeamSbred);
	gsDeleteSkinMesh(Mesh_Skybox);
	gsDeleteSkinMesh(Mesh_EnemyShip);
	gsDeleteSkinMesh(Mesh_PlayerShip);
	//オクトリーの削除
	gsDeleteOctree(Octree_Stage);
	//スカイドームの削除
	gsDeleteMesh(Octree_Collider);

	gsDeleteOctree(Octree_Stage2);
	gsDeleteMesh(Octree_Collider2);

	gsDeleteTexture(Texture_ResultBuck);

}

void GamePlayScene::updateGameScene(float delta_time) {

	//ワールド更新
	world_.update(delta_time);

	//ボスが死んだあとエンター押したらゲーム終了
	if (gsGetKeyTrigger(GKEY_RETURN) && world_.gameData()->bossDie() == true) {
		state_ = State::ResultScene;
	}
}

void GamePlayScene::updateResultScene(float delta_time) {

	//タイトルに戻る
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}
}
