#include "EnemyShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "BOSS/Boss.h"
#include "EnemyAI/TankAI.h"

const float EnemyShipRadius_{ 0.8f };
const float EnemyShipHeight_{ 1.f };

EnemyShip::EnemyShip(IWorld* world, const GSvector3& position) {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "EnemyShip";

	collider_ = BoundingSphere{ EnemyShipRadius_,GSvector3{0,EnemyShipHeight_,0} };

	transform_.position(position);

	boss_ = static_cast<Boss*>(world_->find_actor("Boss"));

	//tankai_ = static_cast<TankAI*>(world_->find_actor("TankAI"));

}

void EnemyShip::update(float delta_time){

	MakeEnemy(delta_time);

}

void EnemyShip::draw() const {

	glRotatef(-90, 0, 1, 0);
	gsDrawMesh(Mesh_EnemyShip);

}

void EnemyShip::react(Actor& other) {

}


void EnemyShip::MakeEnemy(float delta_time){
	
	MakeTimer -= delta_time;

	if (MakeTimer <= 0) {

		//íŽÔAI¶¬
		world_->add_actor(new TankAI{ world_,transform_.position() });

		//ƒ‰ƒ“ƒ_ƒ€‚ÈŽžŠÔ‚ð‘ã“ü
		MakeTimer = gsRandf(MakeTimerRand.x, MakeTimerRand.y);
	}

}
