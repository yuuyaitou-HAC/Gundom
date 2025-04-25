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
#include "EnemyAI/EnemyAttackControl.h"
#include <GSgame.h>
#include "GSeffect.h"
//開始
void GamePlayScene::start() {
	gsInitDefaultShader();
	//フィールドの追加
	world_.add_field(new Field{ Octree_Stage2,Octree_Collider2,Texture_Skybox });

	//カメラ
	world_.add_camera(new CameraTPS{ &world_,GSvector3{204,-8,8.5},GSvector3{0.0f,0.0f,0.0f} });
	//ライトの追加
	world_.add_light(new Light{ &world_ });

	world_.add_gamedata(new GameData{});

	//プレイヤーの追加
	world_.add_actor(new Player{ &world_,GSvector3{204,-8,8.5} });

	//弾管理クラス
	world_.add_actor(new GunControl{ &world_,GSvector3{0.f,0.f,0.f} });

	//敵の弾管理クラス
	world_.add_actor(new EnemyAttackControl{ &world_,GSvector3::zero() });

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

	// 視錐台カリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);

	//初期化
	world_.gameData()->initialize();

	//ゲームシーン開始
	state_ = State::GameScene;

	result_ = new ResultScene{ &world_ };
}

//更新
void GamePlayScene::update(float delta_time) {


	if (gsGetKeyTrigger(GKEY_P)) {
		if (!pauseFrag) {
			pauseFrag = true;
		}
		else {
			pauseFrag = false;
		}
	}

	if (pauseFrag)return;

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
	if (gsGetKeyTrigger(GKEY_L)) state_ = State::ResultScene;

	if (world_.gameData()->playerDie()) {
		state_ = State::ResultScene;
	}

	//ゲーム終了
	//if (gsGetKeyTrigger(GKEY_O)) is_end_ = true;
}

//描画
void GamePlayScene::draw()const {

	if (pauseFrag) {
		gsTextPos(500, 500);
		gsDrawText("pouse中");
	}

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

	// 再生中の全エフェクトを停止（削除）する
	gsStopAllEffects();

	delete result_;

	// メッシュの削除
	gsDeleteSkinMesh(Mesh_Skybox);
	gsDeleteSkinMesh(Mesh_Player);
	gsDeleteSkinMesh(Mesh_HBM);
	gsDeleteSkinMesh(Mesh_Boss);
	gsDeleteSkinMesh(Mesh_Enemy);
	gsDeleteSkinMesh(Mesh_Weapon);
	gsDeleteSkinMesh(Mesh_BeamSbred);
	gsDeleteSkinMesh(Mesh_BeamSbred2);
	gsDeleteSkinMesh(Mesh_EnemyShip);
	gsDeleteSkinMesh(Mesh_MissileBullet);
	gsDeleteSkinMesh(Mesh_PlayerShip);
	gsDeleteSkinMesh(Mesh_AllRangeUnit);
	gsDeleteOctree(Octree_Stage);
	gsDeleteMesh(Octree_Collider);
	gsDeleteOctree(Octree_Stage2);
	gsDeleteMesh(Octree_Collider2);

	gsDeleteTexture(Texture_Skybox);
	gsDeleteTexture(Texture_ResultBuck);
	gsDeleteTexture(Texture_EX1);
	gsDeleteTexture(Texture_EX2);
	gsDeleteTexture(Texture_EX3);
	gsDeleteTexture(Texture_EX4);
	gsDeleteTexture(Texture_EX1Ball);
	gsDeleteTexture(Texture_EX2Ball);
	gsDeleteTexture(Texture_EX3Ball);
	gsDeleteTexture(Texture_EX4Ball);
	gsDeleteTexture(Texture_Buster1);
	gsDeleteTexture(Texture_Buster2);
	gsDeleteTexture(Texture_HP);
	gsDeleteTexture(Texture_HPBack);
	gsDeleteTexture(Texture_BeamLifle);
	gsDeleteTexture(Texture_BeamMagnum);
	gsDeleteTexture(Texture_Bazooka);
	gsDeleteTexture(Texture_Bullet);
	gsDeleteTexture(Texture_Magajin);
	gsDeleteTexture(Texture_Reticle);
	gsDeleteTexture(Texture_MissionBack);
	gsDeleteTexture(Texture_KillNum);
	gsDeleteTexture(Texture_Slash);
	gsDeleteTexture(Texture_UnderBossadvent);
	gsDeleteTexture(Texture_UnderBossHP);
	gsDeleteTexture(Texture_UnderBossKill);
	gsDeleteTexture(Texture_Mission1);
	gsDeleteTexture(Texture_Mission2);
	gsDeleteTexture(Texture_Number);
	gsDeleteTexture(Texture_MissionDescription);
	gsDeleteTexture(Texture_Mission3);
	gsDeleteTexture(Texture_Mission4);
	gsDeleteTexture(Texture_BossMake);
	gsDeleteTexture(Texture_Enter);

	//エフェクトの削除
	gsDeleteEffect(Effect_PBeamRifle);
	gsDeleteEffect(Effect_PBeamMagnum);
	gsDeleteEffect(Effect_EnemyBullet);

	gsDeleteEffect(Effect_Ballistic);
	gsDeleteEffect(Effect_VernierBL);
	gsDeleteEffect(Effect_VernierBS);

	gsDeleteEffect(Effect_VernierBSS);
	gsDeleteEffect(Effect_FootDust);
	gsDeleteEffect(Effect_ExplosionL);

	gsDeleteEffect(Effect_EXBuff);
	gsDeleteEffect(Effect_aura);
	gsDeleteEffect(Effect_DropPoint);

	gsDeleteEffect(Effect_FootDustL);
	gsDeleteEffect(Effect_Impact);
	gsDeleteEffect(Effect_SlashGray);

	gsDeleteEffect(Effect_DarckArrow);
	gsDeleteEffect(Effect_Hit);

	is_end_ = false;
}

void GamePlayScene::updateGameScene(float delta_time) {

	//ワールド更新
	world_.update(delta_time);

	//ボスが死んだあとエンター押したらゲーム終了
	if (gsGetKeyTrigger(GKEY_RETURN) && world_.gameData()->missionClear() == 4) {
		state_ = State::ResultScene;
	}
}

void GamePlayScene::updateResultScene(float delta_time) {

	//タイトルに戻る
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}
}
