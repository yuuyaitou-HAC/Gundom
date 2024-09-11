#pragma once

#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
	Mesh_Player, //自機
	Mesh_Enemy,  //敵
	Mesh_Weapon, //武器
	Mesh_BeamSbred,//ビームサーベル
	Mesh_Skybox, //スカイボックス
};

//オクトリー
enum {
	Octree_Stage,    //ステージ用オクトリー
	Octree_Collider, //衝突判定用オクトリー
};


#endif // !ASSETS_H_
