#pragma once

#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
	Mesh_Player,	//自機
	Mesh_Enemy,		//敵
	Mesh_HBM,		//人型の敵
	Mesh_Boss,		//ボス
	Mesh_underBoss,//中ボス
	Mesh_Weapon,	//武器
	Mesh_BeamSbred,	//ビームサーベル
	Mesh_BeamSbred2,
	Mesh_Skybox,	//スカイボックス
	Mesh_EnemyShip,	//戦艦
	Mesh_PlayerShip,//味方戦艦
	Mesh_AllRangeUnit,
	Mesh_MissileBullet,
	Mesh_GatringBullet,//ミサイル
	Mesh_GoldWheel,//金の輪
};

//オクトリー
enum {
	Octree_Stage,    //ステージ用オクトリー
	Octree_Collider, //衝突判定用オクトリー

	Octree_Stage2,    //ステージ用オクトリー
	Octree_Collider2, //衝突判定用オクトリー
};

//画像
enum {
	Texture_ResultBuck,	//リザルト画面の背景
	Texture_Load,		//ロード中の表示
	Texture_EX1,		//EXスキル
	Texture_EX2,		//EXスキル
	Texture_EX3,		//EXスキル
	Texture_EX4,		//EXスキル
	Texture_EX1Ball,	//EXスキルの弾
	Texture_EX2Ball,	//EXスキルの弾
	Texture_EX3Ball,	//EXスキルの弾
	Texture_EX4Ball,	//EXスキルの弾
	Texture_Buster1,	//スラスター残量
	Texture_Buster2,	//スラスター残量
	Texture_HP,			//HPバー
	Texture_HPBack,		//HPバーの後ろ
	Texture_BeamLifle,	//ビームライフル
	Texture_BeamMagnum,	//ビームマグナム
	Texture_Bazooka,	//バズーカ
	Texture_Bullet,		//弾
	Texture_Magajin,	//マガジン
	Texture_Reticle,//レティクル
	Texture_Title,//タイトル画面
	Texture_MissionBack,//ミッションの背景画像


};

enum {
	Effect_PBeamRifle,	//プレイヤーのビームライフル
	Effect_PBeamMagnum,	//プレイヤーのビームマグナム
	Effect_EnemyBullet,	//仮の敵の弾エフェクト
	Effect_VernierBL,	//バーニアエフェクト青L
	Effect_VernierBS,	//バーニアエフェクト青S
	Effect_VernierBSS,	//バーニアエフェクト青SS
	Effect_FootDust,	//砂埃
	Effect_FootDustL,	//大きい砂埃
	Effect_Ballistic,	//ミサイルの弾道
	Effect_ExplosionL,	//爆発L
	Effect_EXBuff,		//EXスキル発動時のエフェクト	
	Effect_aura,
	Effect_DropPoint,	//落下予想地点のエフェクト
	Effect_Impact,
	Effect_SlashGray,	//斬撃白灰色
	Effect_DarckArrow,	//ダークアロー
	Effect_Hit,			//当たり判定
};
#endif // !ASSETS_H_
