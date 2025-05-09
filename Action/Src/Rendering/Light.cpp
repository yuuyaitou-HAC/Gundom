#include "Rendering/Light.h"

//コンストラクタ
Light::Light(IWorld* world) {
	world_ = world;
	name_ = "Light";
	tag_ = "Lightting";
}

//描画
void Light::draw()const {
	float ambient[]  { 0.f,0.f,0.f,1.f };
	float diffuse[]  { 1.f,1.f,1.f,1.f };
	float specular[] { 1.f,1.f,1.f,1.f };
	float position[] { 100.f,100.f,-150.f,0.f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//
	GSvector3 shadow_map_light_position{ position[0],position[1],position[2] };
	gsSetShadowMapLightPosition(&shadow_map_light_position);
}
