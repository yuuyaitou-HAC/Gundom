#pragma once

#ifndef LINE_H_
#define LINE_H_

#include <gslib.h>

//線分クラス
class Line {
public:
	//デフォルトコンストラクタ
	Line() = default;
	//コンストラクタ
	Line(const GSvector3& start, const GSvector3& end) :
		start{ start }, end{ end }{}


public:
	//始点
	GSvector3 start{ 0.f,0.f,0.f };
	//終点
	GSvector3 end{ 0.f,0.f,0.f };
};

#endif // !LINE_H_
