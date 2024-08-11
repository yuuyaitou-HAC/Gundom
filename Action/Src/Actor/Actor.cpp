#include"Actor/Actor.h"

//更新
void Actor::update(float) {};

//遅延更新
void Actor::late_update(float) {};

//描画
void Actor::draw()const {};

//半透明オブジェクトの描画
void Actor::draw_transparent()const {};

//GUIの描画
void Actor::draw_gui()const {};

//衝突リアクション
void Actor::react(Actor&) {};

//メッセージ処理
void Actor::handle_message(const std::string& message, void* param) {};

//衝突判定
void Actor::collide(Actor& other) {
	//どちらのアクターも衝突判定が有効か？
	if (enable_collider_ && other.enable_collider_) {
		//衝突判定をする
		if (is_collider(other)) {
			//衝突した場合は、お互いに衝突判定をする
			react(other);
			other.react(*this);
		}
	}
}

//死亡する
void Actor::die() {
	dead_ = true;
}

//衝突しているか？
bool Actor::is_collider(const Actor& other)const {
	return collider().intersects(other.collider());
}

//死亡しているか
bool Actor::is_dead()const {
	return dead_;
}

//名前の所得
const std::string& Actor::name()const {
	return name_;
}

//タグ名を所得
const std::string& Actor::tag()const {
	return tag_;
}

//トランスフォームを所得(const版)
const GStransform& Actor::transform()const {
	return transform_;
}

//トランスフォームを所得
GStransform& Actor::transform() {
	return transform_;
}

//移動量を所得
GSvector3 Actor::velocity()const {
	return velocity_;
}

//衝突判定データを所得
BoundingSphere Actor::collider()const {
	return collider_.transform(transform_.localToWorldMatrix());
}
