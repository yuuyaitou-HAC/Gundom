#ifndef WORLD_H_
#define WORLD_H_

#include "World/IWorld.h"
#include "Actor/ActorManager.h"

#include <gslib.h>

//ワールドクラス
class World :public IWorld {
public:
	//コンストラクタ
	World() = default;
	//デストラクタ
	~World();
	//更新
	void update(float delta_time);
	//描画
	void draw()const;
	//消去
	void clear();
	//カメラの追加
	void add_camera(Actor* camera);
	//ライトの追加
	void add_light(Actor* light);
	//フィールドの追加
	void add_field(Field* field);
	void add_gamedata(GameData* gamedata);

	//アクターの追加
	virtual void add_actor(Actor* actor)override;
	//アクターの検索
	virtual Actor* find_actor(const std::string& name)const override;
	//指定したタグ名を持つアクターの検索
	virtual std::vector<Actor*>find_actor_with_tag(const std::string& tag)const override;
	//アクター数を返す
	virtual int count_actor()const override;
	//指定したタグ名を持つアクター数を返す
	virtual int count_actor_with_tag(const std::string& tag)const override;

	virtual GSvector3 find_first_intersection(GSvector3 position, GSvector3 direction)const;

	//メッセージの送信
	virtual void send_message(const std::string& message, void* param = nullptr)override;

	//カメラの所得
	virtual Actor* camera()override;
	//ライトの所得
	virtual Actor* light()override;
	//フィールドの所得
	virtual Field* field()override;

	virtual GameData* gameData()override;

	//シャドウマップの描画用の関数
	static void shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*);

	//コピー禁止
	World(const World& other) = default;
	World& operator=(const World& other) = delete;

private:
	//アクターマネージャー
	ActorManager actors_;
	//ライト
	Actor* light_{ nullptr };
	//カメラ
	Actor* camera_{ nullptr };
	//フィールド
	Field* field_{ nullptr };

	GameData* gamedata_{ nullptr };

};

#endif

