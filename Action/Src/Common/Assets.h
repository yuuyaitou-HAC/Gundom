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
	Texture_Enter,		//タイトルのエンターキー誘導
	Texture_Mission1,	//ミッション内容
	Texture_Mission2,	//ミッション内容
	Texture_Mission3,	//ミッション内容
	Texture_Mission4,	//ミッション内容
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
	Texture_Skybox,//スカイボックス
	Texture_Number,//数字
	Texture_KillNum,//撃破数
	Texture_Slash,//これ→/
	Texture_UnderBossadvent,//中ボス出現
	Texture_UnderBossHP,//中ボスのHP
	Texture_UnderBossKill,//中ボス死亡時に出す物
	Texture_MissionDescription,//ミッション３の導入
	Texture_BossMake,	//ボス登場時のテキスト
	Texture_GameClear,	//ゲームクリア時に表示
	Texture_Killnum2,	//ミッション３の撃破数
	Texture_missiontimer,//ミッション３のミッション時間
	Texture_bossHP,//ボスのHP
	Texture_Clon,// これ→:
	Texture_Manual1,//操作説明1
	Texture_Manual2,//操作説明2
	Texture_Manual3,//操作説明3
	Texture_Manual4,//操作説明4
	Texture_Manual5,//操作説明5
	Texture_Manual6,//操作説明6
	Texture_Manual7,//操作説明7
	Texture_Manual8,//操作説明8
	Texture_Manual9,//操作説明9
	Texture_Manual10,//操作説明10
	Texture_Manual11,//操作説明11
	Texture_Manual12,//操作説明12
	Texture_Manual13,//操作説明13
	Texture_Manual14,//操作説明14
	Texture_Manual15,//操作説明15
	Texture_dounyu1,
	Texture_dounyu2,
	Texture_dounyu3,

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
