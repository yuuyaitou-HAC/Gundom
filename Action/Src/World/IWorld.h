#ifndef IWORLD_H_
#define IWORLD_H_

#include <string>
#include <vector>

class Actor;
class Field;

class IWorld {
public:
	//仮想デストラクタ
	virtual ~IWorld() = default;

	//アクターを追加
	virtual void add_actor(Actor* actor) = 0;
	//アクターの検索
	virtual Actor* find_actor(const std::string& name)const = 0;
	//指定したタグ名を持つアクターの検索
	virtual std::vector<Actor*>find_actor_with_tag(const std::string& tag)const = 0;
	//アクター数を返す
	virtual int count_actor()const = 0;
	//指定したタグ名を持つアクター数を返す
	virtual int count_actor_with_tag(const std::string& tag)const = 0;
	//メッセージの送信
	virtual void send_message(const std::string& message, void* param = nullptr) = 0;

	//フィールドの所得
	virtual Field* field() = 0;
	//カメラの所得
	virtual Actor* camera() = 0;
	//ライトの所得
	virtual Actor* light() = 0;
};

#endif // !IWORLD_H_