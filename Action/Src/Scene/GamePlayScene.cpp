#include "Scene/GamePlayScene.h"
#include "Field/Field.h"
#include "Camera/CameraTPS.h"
#include "Rendering/Light.h"
#include "Player/Player.h"
#include "Common/Assets.h"
#include "Common/GameData.h"
#include "Gun/GunControl.h"
#include <GSstandard_shader.h>
#include "BattleShip/EnemyShip.h"

#include "BOSS/Boss.h"
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

	gsLoadMesh(Mesh_BeamSbred2, "Assets/BeamSabre2/BeamSabre2.mshb");


	//敵戦艦を読み込む
	gsLoadMesh(Mesh_EnemyShip, "Assets/EnemyShip/EnemyShip.mshb");

	//オクトリーを読み込む
	//gsLoadOctree(Octree_Stage, "Assets/Octree/stage.oct");
	//衝突判定用のオクトリーを読み込む
	//gsLoadOctree(Octree_Collider, "Assets/Octree/stage_collider.oct");
	//ライトマップの読み込み
	//gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
	//リフレクションプローブの読み込み
	//gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");

	//今のステージ
	gsLoadOctree(Octree_Stage2, "Assets/Stage2/Octree/Octree.oct");
	gsLoadOctree(Octree_Collider2, "Assets/Stage2/ColliderMesh/Collider.oct");
	gsLoadLightmap(0, "Assets/Stage2/Lightmap/Lightmap.txt");
	gsLoadReflectionProbe(0, "Assets/Stage2/RefProbe/ReflectionProbe.txt");

	//テストステージ
	//gsLoadOctree(Octree_Stage2, "Assets/testStege/Octree/testStage.oct");
	//gsLoadOctree(Octree_Collider2, "Assets/testStege/ColliderMesh/testStage.oct");
	//gsLoadLightmap(0, "Assets/testStege/Lightmap/Lightmap.txt");
	//gsLoadReflectionProbe(0, "Assets/testStege/RefProbe/ReflectionProbe.txt");

	//フィールドの追加
	world_.add_field(new Field{ Octree_Stage2,Octree_Collider2,Mesh_Skybox });

	//カメラ
	world_.add_camera(new CameraTPS{ &world_,GSvector3{-77.f,0.f,-5.f},GSvector3{0.0f,1.7f,0.0f} });
	//ライトの追加
	world_.add_light(new Light{ &world_ });

	world_.add_gamedata(new GameData{});

	//プレイヤーの追加
	world_.add_actor(new Player{ &world_,GSvector3{-77.f,0.f,-5.f} });

	//弾管理クラス
	world_.add_actor(new GunControl{ &world_,GSvector3{0.f,0.f,0.f} });

	//world_.add_actor(new Boss{ &world_,GSvector3{-77.f,0.f,-5.f} });

	//戦艦
	world_.add_actor(new EnemyShip{ &world_,GSvector3{122.2,10,-10} });

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
}

//更新
void GamePlayScene::update(float delta_time) {

	bool trigger = world_.gameData()->bossDie();

	//ボスが死んだあとエンター押したらゲーム終了
	if (gsGetKeyTrigger(GKEY_RETURN) && trigger == true) {
		is_end_ = true;
	}

	//デバッグ用の終了
	if (gsGetKeyTrigger(GKEY_P)) {
		is_end_ = true;
	}

	//ワールドの更新
	world_.update(delta_time);
}

//描画
void GamePlayScene::draw()const {
	//ワールドの描画
	world_.draw();
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

	// メッシュの削除
	gsDeleteSkinMesh(Mesh_Player);
	gsDeleteSkinMesh(Mesh_Enemy);
	gsDeleteSkinMesh(Mesh_Boss);
	gsDeleteSkinMesh(Mesh_HBM);
	gsDeleteSkinMesh(Mesh_Weapon);
	gsDeleteSkinMesh(Mesh_BeamSbred);
	gsDeleteSkinMesh(Mesh_Skybox);
	gsDeleteSkinMesh(Mesh_EnemyShip);
	//オクトリーの削除
	gsDeleteOctree(Octree_Stage);
	//スカイドームの削除
	gsDeleteMesh(Octree_Collider);

	gsDeleteOctree(Octree_Stage2);
	gsDeleteMesh(Octree_Collider2);

}