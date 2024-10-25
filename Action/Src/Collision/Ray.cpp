#ifndef RAY_H_
#define RAY_H_

#include <gslib.h>

class Ray {
public:

	//デフォルトコンストラクタ
	Ray() = default;

	//コンストラクタ	
	Ray(const GSvector3& position, const GSvector3& direction) :
		position{ position }, direction{ direction } {}

public:
	//座標
	GSvector3 position{ 0.f,0.f,0.f };

	//方向
	GSvector3 direction{ 0.f,0.f,0.f };

};

#endif // !RAY_H_
