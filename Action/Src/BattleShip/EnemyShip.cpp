#include "EnemyShip.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "Common/Assets.h"
#include "BOSS/Boss.h"
#include "EnemyAI/TankAI.h"
#include "Collision/Ray.h"


const float EnemyShipRadius_{ 0.8f };
const float EnemyShipHeight_{ 1.f };

const float Hight_{ 1.f };

EnemyShip::EnemyShip(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyShip,Mesh_EnemyShip ,Mesh_EnemyShip ,0 },
	motion_{ 0 },
	motion_loop_{ true } {

	world_ = world;

	tag_ = "ShipTag";
	name_ = "EnemyShip";

	collider_ = BoundingSphere{ EnemyShipRadius_,GSvector3{0,EnemyShipHeight_,0} };

	transform_.position(position);

	boss_ = static_cast<Boss*>(world_->find_actor("Boss"));

	//tankai_ = static_cast<TankAI*>(world_->find_actor("TankAI"));

}

void EnemyShip::update(float delta_time) {

	//自身の座標を取得
	pos = transform_.position();

	//モーション更新
	mesh_.ChangeMotion(motion_, motion_loop_);

	//メッシュを更新
	mesh_.Update(delta_time);

	//行列を設定	
	mesh_.Transform(transform_.localToWorldMatrix());

	MakeEnemy(delta_time);

}

void EnemyShip::draw() const {

	mesh_.Draw();

	gsTextPos(200, 500);
	gsDrawText("pos = %f,%f,%f", transform_.position().x, transform_.position().y, transform_.position().z);


}

void EnemyShip::react(Actor& other) {

}


void EnemyShip::MakeEnemy(float delta_time) {

	//生成座標の設定
	Ray ray = { transform_.position(),-(transform_.up()) };
	Spawnpoint = pos;
	Spawnpoint.y = ray.position.y + Hight_;

	MakeTimer -= delta_time;

	if (MakeTimer <= 0) {

		//戦車AI生成
		world_->add_actor(new TankAI{ world_,Spawnpoint });

		//ランダムな時間を代入
		MakeTimer = gsRand(MakeTimerRand.x, MakeTimerRand.y);
	}

}
