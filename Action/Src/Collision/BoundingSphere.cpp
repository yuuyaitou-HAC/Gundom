#include "Collision/BoundingSphere.h"


// コンストラクタ
BoundingSphere::BoundingSphere(float radius, const GSvector3& center) :
	radius{ radius }, center{ center }{
}

// 平行移動
BoundingSphere BoundingSphere::translate(const GSvector3& position)const {
	return BoundingSphere{ radius,center + position };
}

// 座標変換
BoundingSphere BoundingSphere::transform(const GSmatrix4& matrix)const {
	return BoundingSphere{ radius * matrix.getScale().y,matrix.transform(center) };
}

// 交差しているか？
bool BoundingSphere::intersects(const BoundingSphere& other)const {
	return gsCollisionSphereAndSphere(
		&center, radius, &other.center, other.radius) == GS_TRUE;
}

// デバッグ表示
void BoundingSphere::draw()const {
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
	// ワイヤーフレームの球体を描画
	glutWireSphere(radius, 16, 16);
	glPopMatrix();
}
