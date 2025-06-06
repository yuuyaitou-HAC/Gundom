#ifndef PLAYER_BULLET_H_
#define PLAYER_BULLET_H_

#include "Collision/BasicAttackCollider.h"
#include "Player/PlayerState.h"

//プレイヤーの弾クラス
class PlayerBullet :public BasicAttackCollider {
public:
	//コンストラクタ
	PlayerBullet(IWorld* world, const GSvector3& positon, const GSvector3& velocity, int Damage, std::string name);
	~PlayerBullet();
	//更新
	virtual void update(float delta_time)override;

	//衝突リアクション
	virtual void react(Actor& other)override;

private:
	//寿命
	float lifeSpanTimer_;

	//エフェクトハンドル
	GSint bulletEffect_;

	GSquaternion quatenion_;

	GSmatrix4 effectSize_;
	GSvector3 bulletEffectScale_{4,4,2};
};
#endif