#include "Scene/GamePlayScene.h"
#include "Field/Field.h"
#include "Camera/Camera.h"
#include "Camera/CameraTPS.h"
#include "Rendering/Light.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Enemy/Tank.h"
#include "Common/Assets.h"
#include "Gun/GunControl.h"
#include <GSstandard_shader.h>
#include "EnemyAI/TankAI.h"

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
	//gsLoadSkinMesh(Mesh_Player, "Assets/Player/player.msh");
	gsLoadSkinMesh(Mesh_Player, "Assets/Robo2/Player.msh");
	//敵メッシュの読み込み
	//gsLoadSkinMesh(Mesh_Enemy, "Assets/Enemy/ghoul.msh");

	gsLoadSkinMesh(Mesh_Enemy, "Assets/sennsya/Sennsya.msh");
	//武器のメッシュを読み込む
	gsLoadMesh(Mesh_Weapon, "Assets/Weapon/w_magun01.msh");

	//ビームサーベルを読み込む
	gsLoadMesh(Mesh_BeamSbred, "Assets/BeamSabre/BeamSabre.mshb");

	//オクトリーを読み込む
	gsLoadOctree(Octree_Stage, "Assets/Octree/stage.oct");
	//衝突判定用のオクトリーを読み込む
	gsLoadOctree(Octree_Collider, "Assets/Octree/stage_collider.oct");

	//ライトマップの読み込み
	gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
	//リフレクションプローブの読み込み
	gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");

	//フィールドの追加
	world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
	//カメラの追加
	//world_.add_camera(new Camera{ &world_ });
	world_.add_camera(new CameraTPS{ &world_,GSvector3{0.0f,3.0f,-5.0f},GSvector3{0.0f,1.7f,0.0f} });
	//ライトの追加
	world_.add_light(new Light{ &world_ });

	//プレイヤーの追加
	world_.add_actor(new Player{ &world_,GSvector3{0.f,0.f,0.f} });

	//敵AIの追加
	world_.add_actor(new TankAI{ &world_,GSvector3{-0.f,0.f,0.f} });
	world_.add_actor(new TankAI{ &world_,GSvector3{-10.f,0.f,10.f} });

	//弾管理クラス
	world_.add_actor(new GunControl{ &world_,GSvector3{0.f,0.f,0.f} });


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
	//エンターキーで終了としておく
	if (gsGetKeyTrigger(GKEY_RETURN)) {
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
	gsDeleteSkinMesh(Mesh_Weapon);
	gsDeleteSkinMesh(Mesh_BeamSbred);
	gsDeleteSkinMesh(Mesh_Skybox);
	//オクトリーの削除
	gsDeleteOctree(Octree_Stage);
	//スカイドームの削除
	gsDeleteMesh(Octree_Collider);
}