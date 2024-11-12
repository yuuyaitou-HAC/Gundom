#pragma once

#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
	Mesh_Player,	//自機
	Mesh_Enemy,		//敵
	Mesh_HBM,		//人型の敵
	Mesh_Boss,		//ボス
	Mesh_Weapon,	//武器
	Mesh_BeamSbred,	//ビームサーベル
	Mesh_Skybox,	//スカイボックス
	Mesh_EnemyShip,	//戦艦
};

//オクトリー
enum {
	Octree_Stage,    //ステージ用オクトリー
	Octree_Collider, //衝突判定用オクトリー

	Octree_Stage2,    //ステージ用オクトリー
	Octree_Collider2, //衝突判定用オクトリー
};


#endif // !ASSETS_H_
