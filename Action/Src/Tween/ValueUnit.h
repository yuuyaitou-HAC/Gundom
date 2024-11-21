#ifndef VALUE_UNIT_H_
#define VALUE_UNIT_H_

#include "TweenUnit.h"

//値を動かすためのTweenUnit
class ValueUnit : public TweenUnit {

public:
	//コンストラクタ
	ValueUnit(float from, float to, float duration,
		std::function<void(float)>update_func);

	//更新時の処理
	virtual void on_update(float progress)override;

private:

	//開始値
	float from_;
	
	//終了値
	float to_;
	
	//更新時の処理
	std::function<void(float)> update_func_;


};

#endif // !VALUE_UNIT_H_
