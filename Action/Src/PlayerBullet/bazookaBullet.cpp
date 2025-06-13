#include "bazookaBullet.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"
#include "DamageRange.h"
#include "Common\Assets.h"
#include "GSeffect.h"

BazookaBullet::BazookaBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, int Damage) :
	mesh_{ Mesh_MissileBullet,Mesh_MissileBullet ,Mesh_MissileBullet } {

	//ワールドを設定
	world_ = world;
	//タグ名
	tag_ = "PlayerBulletTag";
	//アクター名
	name_ = "BazookaBullet";
	//移動量の初期化
	velocity_ = velocity;
	//衝突判定用の球体を設定
	collider_ = BoundingSphere{ 0.2f };

	//回転角度の調整
	GSquaternion a;
	a.setLookRotation(velocity);
	transform_.rotation(a);

	//座標の初期化
	transform_.position(position);
	//寿命
	lifespanTimer_ = 60.f;

	//ダメージ量
	m_AttackValue = Damage;

	mesh_.Transform(transform_.localToWorldMatrix());

	//プレイヤー取得
	player_ = static_cast<Player*>(world_->find_actor("Player"));
	//エフェクトの作成
	vernierEffect_ = gsPlayEffect(Effect_Ballistic, &position);
}

void BazookaBullet::update(float delta_time)
{
	localMatrix_ = GSmatrix4::TRS(vernierEffectPos_, GSquaternion::euler(vernierEffectEuler_), vernierEffectScale_);

	//エフェクトに自身のワールド変換行列を設定
	effectWorld_ = localMatrix_ * transform_.localToWorldMatrix();

	//ワールド変換行列を設定
	gsSetEffectMatrix(vernierEffect_, &effectWorld_);

	mesh_.Update(delta_time);

	//寿命が尽きたら死亡
	if (lifespanTimer_ <= 0.f) {
		gsStopEffect(vernierEffect_);
		//爆風当たり判定生成
		if (!explosion_) {
			world_->add_actor(new DamageRange{ world_,transform_.position(),GSvector3().zero(),player_->player_state()->attack() * 4 });
			explosion_ = true;
		}

		die();
		return;
	}
	//寿命の更新
	lifespanTimer_ -= delta_time;
	//フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_ * delta_time;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		//爆風当たり判定生成
		if (!explosion_) {
			world_->add_actor(new DamageRange{ world_,transform_.position(),GSvector3().zero(),player_->player_state()->attack() * 4 });
			explosion_ = true;
		}

		//交点の座標に補正
		transform_.position(intersect);
		gsStopEffect(vernierEffect_);
		//フィールドに衝突したら死亡
		die();
		return;
	}
	//移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);

	mesh_.Transform(transform_.localToWorldMatrix());

}

void BazookaBullet::draw() const {
	mesh_.Draw();
}

void BazookaBullet::react(Actor& other)
{
	if (other.tag() == "EnemyTag") {
		//爆風当たり判定生成
		if (!explosion_) {
			world_->add_actor(new DamageRange{ world_,transform_.position(),GSvector3().zero(),player_->player_state()->attack() * 4 });
			explosion_ = true;
		}
		gsStopEffect(vernierEffect_);
		//衝突したら死亡
		die();
	}
}