#include "World/World.h"
#include "Field/Field.h"
#include "Actor/Actor.h"
#include "Collision/Ray.h"
#include <algorithm>
#include "Common/GameData.h"

//デストラクタ
World::~World() {
	clear();
}

//更新
void World::update(float delta_time) {
	//フィールドの更新
	field_->update(delta_time);
	//アクターの更新
	actors_.update(delta_time);
	//アクターの衝突
	actors_.collide();
	//アクターの遅延更新
	actors_.late_update(delta_time);
	//アクターの消去
	actors_.remove();
	//カメラの更新
	camera_->update(delta_time);
	//ライトの更新
	light_->update(delta_time);
}

//描画
void World::draw() const {
	//カメラの描画
	camera_->draw();
	//ライトの描画
	light_->draw();

	//シャドウマップの描画
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);

	//フィールドの描画
	field_->draw();
	//アクターの描画
	actors_.draw();
	//半透明のアクターの描画
	actors_.draw_transparent();
	//GUIの描画
	actors_.draw_gui();
}

//消去
void World::clear() {
	//アクターを消去
	actors_.clear();
	//カメラを消去
	delete camera_;
	camera_ = nullptr;
	//ライトを消去
	delete light_;
	light_ = nullptr;
	//フィールドを消去
	delete field_;
	field_ = nullptr;
	delete gamedata_;
	gamedata_ = nullptr;
}

//カメラの追加
void World::add_camera(Actor* camera) {
	delete camera_;     //現在のカメラを削除
	camera_ = camera;
}

//ライトの追加
void World::add_light(Actor* light) {
	delete light_;      //現在のライトを削除
	light_ = light;
}

//フィールドの追加
void World::add_field(Field* field) {
	delete field_;      //現在のフィールドを削除
	field_ = field;
}

void World::add_gamedata(GameData* gamedata) {

	delete gamedata_;
	gamedata_ = gamedata;
}

//アクターの追加
void World::add_actor(Actor* actor) {
	actors_.add(actor);
}

//アクターの検索
Actor* World::find_actor(const std::string& name)const {
	return actors_.find(name);
}

//指定したタグ名を持つアクターの検索
std::vector<Actor*>World::find_actor_with_tag(const std::string& tag)const {
	return actors_.find_with_tag(tag);
}

//アクター数を返す
int World::count_actor()const {
	return actors_.count();
}

//指定したタグ名を持つアクター数を返す
int World::count_actor_with_tag(const std::string& tag)const {
	return actors_.count_with_tag(tag);
}

GSvector3 World::find_first_intersection(GSvector3 position, GSvector3 direction) const {

	Ray ray = { position,direction };
	std::vector<std::pair<float, GSvector3>> storage;
	GSvector3 intersect;

	for (auto& actor : actors_.actors()) {
		BoundingSphere sphre = actor->collider();
		if (gsCollisionSphereAndRay(&sphre.center, sphre.radius, &ray.position, &ray.direction, &intersect) == GS_TRUE)
		{
			//プレイヤータグ、ファンネル管轄タグ、敵タグ、当たり判定タグを持つ物とは衝突しない
			if (actor->tag() != "PlayerTag" && actor->tag() != "ControlUnitsTag" && actor->tag() != "EnemyTag" && actor->tag() != "CollisionDerectionTag")
			{
				std::pair<float, GSvector3> p = { GSvector3::distance(ray.position,intersect),intersect };
				storage.push_back(p);
			}
		}
	}

	// フィールドとの衝突判定
	GSplane plane;
	if (field_->collide(ray, 100, &intersect, &plane)) {
		std::pair<float, GSvector3> p = { GSvector3::distance(ray.position,intersect),intersect };
		storage.push_back(p);
	}

	std::sort(storage.begin(), storage.end());

	if (storage.size() == 0)
	{
		return position + direction * 100.0f;
	}

	return storage.at(0).second;
}

//メッセージの送信
void World::send_message(const std::string& message, void* param) {
	actors_.send_message(message, param);
}

//カメラの所得
Actor* World::camera() {
	return camera_;
}

//ライトの所得
Actor* World::light() {
	return light_;
}

//フィールドの所得
Field* World::field() {
	return field_;
}

GameData* World::gameData()
{
	return gamedata_;
}

//シャドウマップの描画用の関数
void World::shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*) {
	World* self = (World*)param;
	//シャドウマップにはアクターのみ描画
	self->actors_.draw();
}