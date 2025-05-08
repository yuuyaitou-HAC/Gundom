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
	gsLoadSkinMesh(Mesh_Player, "Assets/Robo2/Player.mshb");
	//人型の敵
	gsLoadSkinMesh(Mesh_HBM, "Assets/EnemyBase/EnemyBase.mshb");
	//ボス
	gsLoadSkinMesh(Mesh_Boss, "Assets/lastBoss/Boss.mshb");

	//中ボス
	gsLoadSkinMesh(Mesh_underBoss, "Assets/Boss/Boss.mshb");
	//戦車のメッシュ
	gsLoadSkinMesh(Mesh_Enemy, "Assets/sennsya/Sennsya.mshb");
	//武器のメッシュを読み込む
	gsLoadMesh(Mesh_Weapon, "Assets/Weapon/w_magun01.msh");

	//ミサイルの弾
	gsLoadMesh(Mesh_MissileBullet, "Assets/Missile/Missile.msh");

	gsLoadMesh(Mesh_GatringBullet, "Assets/Bullet/GatringBullet.msh");
	//ビームサーベルを読み込む
	gsLoadMesh(Mesh_BeamSbred, "Assets/BeamSabre/BeamSabre.msh");
	//ビームサーベル展開時
	gsLoadMesh(Mesh_BeamSbred2, "Assets/BeamSabre2/BeamSabre2.msh");
	//敵戦艦を読み込む
	gsLoadMesh(Mesh_EnemyShip, "Assets/EnemyShip/EnemyShip.msh");
	//味方戦艦を読み込む
	gsLoadMesh(Mesh_PlayerShip, "Assets/PlayerShip/PlayerShip.msh");
	//ファンネル
	gsLoadMesh(Mesh_AllRangeUnit, "Assets/AllRangeUnit/AllRangeUnit.msh");

	//金の輪
	gsLoadMesh(Mesh_GoldWheel, "Assets/GoldWheel/GoldWheel.msh");



	//今のステージ
	gsLoadOctree(Octree_Stage2, "Assets/Stage2/Octree/Octree.oct");
	gsLoadOctree(Octree_Collider2, "Assets/Stage2/ColliderMesh/Collider.oct");
	gsLoadLightmap(0, "Assets/Stage2/Lightmap/Lightmap.txt");
	gsLoadReflectionProbe(0, "Assets/Stage2/RefProbe/ReflectionProbe.txt");

	gsLoadTexture(Texture_Skybox, "Assets/Stage2/skybox/skybox.dds");

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
	gsLoadTexture(Texture_HPBer, "Assets/Texture/HPBar.png");
	gsLoadTexture(Texture_HPBack, "Assets/Texture/HPBar2.png");
	gsLoadTexture(Texture_BeamLifle, "Assets/Texture/beamrifle.png");
	gsLoadTexture(Texture_BeamMagnum, "Assets/Texture/beamMagnam.png");
	gsLoadTexture(Texture_Bazooka, "Assets/Texture/bazooka.png");
	gsLoadTexture(Texture_Bullet, "Assets/Texture/Bullet.png");
	gsLoadTexture(Texture_Magajin, "Assets/Texture/Magazin.png");
	gsLoadTexture(Texture_Reticle, "Assets/Texture/Reticle.png");
	gsLoadTexture(Texture_MissionBack, "Assets/Texture/missionback.png");
	gsLoadTexture(Texture_KillNum, "Assets/Texture/killnum.png");
	gsLoadTexture(Texture_Slash, "Assets/Texture/slash.png");
	gsLoadTexture(Texture_UnderBossadvent, "Assets/Texture/Underbossadvent.png");
	gsLoadTexture(Texture_UnderBossHP, "Assets/Texture/UndreBossHP.png");
	gsLoadTexture(Texture_UnderBossKill, "Assets/Texture/underBossKill.png");
	gsLoadTexture(Texture_Mission1, "Assets/Texture/Mission1.png");
	gsLoadTexture(Texture_Mission2, "Assets/Texture/Mission2.png");
	gsLoadTexture(Texture_Number, "Assets/Texture/number.png");
	gsLoadTexture(Texture_MissionDescription, "Assets/Texture/missionDescription.png");
	gsLoadTexture(Texture_Mission3, "Assets/Texture/mission3.png");
	gsLoadTexture(Texture_Mission4, "Assets/Texture/mission4.png");
	gsLoadTexture(Texture_BossMake, "Assets/Texture/Bossmake.png");
	gsLoadTexture(Texture_GameClear, "Assets/Texture/gameclear.png");
	gsLoadTexture(Texture_bossHP, "Assets/Texture/bosshp.png");
	gsLoadTexture(Texture_Killnum2, "Assets/Texture/killnum2.png");
	gsLoadTexture(Texture_missiontimer, "Assets/Texture/missiontimer.png");
	gsLoadTexture(Texture_Clon, "Assets/Texture/clon.png");

	//マニュアル
	gsLoadTexture(Texture_Manual1, "Assets/Texture/manual.png");
	gsLoadTexture(Texture_Manual2, "Assets/Texture/manual2.png");
	gsLoadTexture(Texture_Manual3, "Assets/Texture/manual3.png");
	gsLoadTexture(Texture_Manual4, "Assets/Texture/manual4.png");
	gsLoadTexture(Texture_Manual5, "Assets/Texture/manual5.png");
	gsLoadTexture(Texture_Manual6, "Assets/Texture/manual6.png");
	gsLoadTexture(Texture_Manual7, "Assets/Texture/manual7.png");
	gsLoadTexture(Texture_Manual8, "Assets/Texture/manual8.png");
	gsLoadTexture(Texture_Manual9, "Assets/Texture/manual9.png");
	gsLoadTexture(Texture_Manual10, "Assets/Texture/manual10.png");
	gsLoadTexture(Texture_Manual11, "Assets/Texture/manual11.png");
	gsLoadTexture(Texture_Manual12, "Assets/Texture/manual12.png");
	gsLoadTexture(Texture_Manual13, "Assets/Texture/manual13.png");
	gsLoadTexture(Texture_Manual14, "Assets/Texture/manual14.png");
	gsLoadTexture(Texture_Manual15, "Assets/Texture/manual15.png");

	gsLoadTexture(Texture_dounyu1, "Assets/Texture/dounyuu1.png");
	gsLoadTexture(Texture_dounyu2, "Assets/Texture/dounyuu2.png");
	gsLoadTexture(Texture_dounyu3, "Assets/Texture/dounyuu3.png");

	gsLoadTexture(Texture_HP, "Assets/Texture/HP.png");
	gsLoadTexture(Texture_EX, "Assets/Texture/EX.png");



	//エフェクト
	gsLoadEffect(Effect_PBeamRifle, "Assets/Effect/Arrow01_Purple.efkefc");		//ビームライフル
	gsLoadEffect(Effect_PBeamMagnum, "Assets/Effect/Arrow02_Purple.efkefc");	//ビームマグナム
	gsLoadEffect(Effect_EnemyBullet, "Assets/Effect/Arrow01_Red.efkefc");		//敵のビームライフルなど

	gsLoadEffect(Effect_Ballistic, "Assets/Effect/Arrow02_Orange.efkefc");		//ミサイルの後ろ
	gsLoadEffect(Effect_VernierBL, "Assets/Effect/Thruster_L_Blue.efkefc");		//プレイヤーのスラスター
	gsLoadEffect(Effect_VernierBS, "Assets/Effect/Thruster_S_Blue.efkefc");		//プレイヤーのスラスター

	gsLoadEffect(Effect_VernierBSS, "Assets/Effect/Thruster_SS_Blue.efkefc");	//プレイヤーのスラスター
	gsLoadEffect(Effect_FootDust, "Assets/Effect/FootDust.efkefc");				//足元の煙
	gsLoadEffect(Effect_ExplosionL, "Assets/Effect/Explosion01_large.efkefc");	//ミサイルやプレイヤーの爆発

	gsLoadEffect(Effect_EXBuff, "Assets/Effect/Buff01-endless-red.efkefc");		//プレイヤーのEXスキル発動時に出ている縦線のやつ
	gsLoadEffect(Effect_aura, "Assets/Effect/Aura02_Red.efkefc");				//EXスキルの赤いもやもや
	gsLoadEffect(Effect_DropPoint, "Assets/Effect/MagicCircle01.efkefc");		//ミサイルの落下予測地点

	gsLoadEffect(Effect_FootDustL, "Assets/Effect/GroundDustLarge.efkefc");		//たたきつけたときに出るでかい砂埃
	gsLoadEffect(Effect_Impact, "Assets/Effect/GroundShock.efkefc");		//たたきつけたときに出る光の柱
	gsLoadEffect(Effect_SlashGray, "Assets/Effect/Slash02_white_gray.efkefc");		//薙ぎ払いの時に出る斬撃

	gsLoadEffect(Effect_DarckArrow, "Assets/Effect/Arrow02_Dark.efkefc");		//ボスのビームライフル
	gsLoadEffect(Effect_Hit, "Assets/Effect/Hit01-m.efkefc");		//ヒットエフェクト

	is_end_ = true;
}
