#include "Field/Field.h"
#include "Collision/Line.h"
#include "Collision/Ray.h"
#include "Collision/BoundingSphere.h"

//
Field::Field(GSuint octree, GSuint collider, GSuint skybox):
	octree_{ octree }, collider_{ collider }, skybox_{ skybox }{
}

//
void Field::update(float delta_time) {}

//
void Field::draw()const {
	//
	gsDrawSkyboxCubemap(skybox_);
	//
	gsDrawOctree(octree_);
}

//
bool Field::collide(const Line& line, GSvector3* intersect, GSplane* plane)const {
	return gsOctreeCollisionLine(
		gsGetOctree(collider_),
		&line.start, &line.end, intersect, plane) == GS_TRUE;
}

//
bool Field::collide(const Ray& ray, float max_distance,
	GSvector3* intersect, GSplane* plane) const {
	Line line{
		 ray.position,
		 ray.position + (ray.direction.normalized() * max_distance) };
	return collide(line, intersect, plane);
}

//
bool Field::collide(const BoundingSphere& sphere, GSvector3* center) const {
	return gsOctreeCollisionSphere(
		gsGetOctree(collider_),
		&sphere.center,sphere.radius, center) == GS_TRUE;
}