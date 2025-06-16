#include "ControlUnits.h"
#include "AllRangeUnit.h"
#include "Player/Player.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

//ユニットの生成数
const int MakeNumber = 5;

ControlUnits::ControlUnits(IWorld* world, const GSvector3& position) :
	units_{ MakeNumber },
	enemys_{ MakeNumber } {

	world_ = world;

	tag_ = "ControlUnitsTag";

	name_ = "ControlUnits";

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

	GSvector3 playerforward = player_->transform().forward() * 10;

	playerforward.y += height_;

	collider_ = BoundingSphere{ radius_,GSvector3{0.f,height_,0.f} };

	makeUnits();
}

void ControlUnits::makeUnits() {

	for (int i = 0; i < MakeNumber; i++) {

		GSvector3 makepos = transform_.position();

		//座標ずらし
		makepos.z += 2 * i;

		units_[i] = new AllRangeUnit{ world_, makepos };
		world_->add_actor(units_[i]);
	}
}

ControlUnits::~ControlUnits() {
	enemys_.clear();
	units_.clear();
	die();
}

void ControlUnits::update(float delta_time) {

	//自身の座標をプレイヤーの座標にする
	transform_.position(player_->transform().position());

	//敵配列の状態更新
	Enemyarraymanagement();

	//自身の座標を更新
	pos_ = transform_.position();

	if (!change_) {
		if (StateNow(AllRangeUnit::State::Attack)) {
			settarget();
		}
	}
	else {
		retreat();
		diechack();
	}
}

void ControlUnits::react(Actor& other) {

	if (other.tag() == "EnemyTag") {

		//判定に当たった敵
		Actor* hitEnemy = static_cast<Actor*>(&other);

		for (auto& enemy : enemys_) {
			//配列内に同じものがあったらパス
			if (enemy == hitEnemy) {
				return;
			}
		}

		//空いているスペースに敵を入れる
		for (auto& enemy : enemys_) {
			if (enemy == NULL) {
				enemy = hitEnemy;
				break;
			}
		}
	}
}

//生成時に対象を割り当てる
void ControlUnits::settarget() {

	for (auto& unit : units_) {
		//ターゲットをまだ所持していたら更新しない
		if (unit->return_target() != NULL)continue;

		Actor* target = PickTarget();

		if (target == NULL)return;

		//敵配列内のランダムな奴とそのタグを渡す
		unit->set_target(target);
	}
}

//撤収
void ControlUnits::retreat() {
	for (auto& unit : units_) {
		unit->change_state(AllRangeUnit::State::Retreat);
	}
}

//全ユニットが指定したステータスに移行したかどうか
bool ControlUnits::StateNow(AllRangeUnit::State state) {

	int count = 0;

	for (auto& unit : units_) {

		if (unit->now_state() == state) {
			count++;
		}
	}

	if (count == MakeNumber)return true;

	return false;
}

//敵の配列管理
void ControlUnits::Enemyarraymanagement() {

	for (auto& enemy : enemys_) {

		//NULLなら飛ばす
		if (enemy == NULL)continue;

		GSvector3 enemypos = enemy->transform().position();

		float dis = GSvector3::distance(enemypos, pos_);

		//保持している個体が死んだもしくは一定の距離以上離れたら除外
		if (dis > radius_ || enemy->tag() != "EnemyTag") {

			enemy = NULL;
		}
	}
}

void ControlUnits::diechack() {

	for (auto& unit : units_) {
		if (unit->tag() != "AllRangeUnitTag")dieCounter_++;
	}

	if (dieCounter_ == MakeNumber)die();
	else dieCounter_ = 0;
}

//ターゲットを渡す
Actor* ControlUnits::PickTarget() {

	Actor* target = enemys_[gsRand(0, MakeNumber - 1)];

	if (target != NULL)return target;

	sarchCounter_++;

	if (sarchCounter_ > 5) {
		sarchCounter_ = 0;
		return NULL;
	}

	return PickTarget();
}

//撤退するかどうかのフラグ
void ControlUnits::changeFrag(bool frag) {
	change_ = frag;
}