#include "ControlUnits.h"
#include "AllRangeUnit.h"
#include "Player/Player.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

//ユニットの生成数
const int MakeNumber = 5;

//高さと幅
const float Height{ 1.f };
const float Radius{ 20.0f };

ControlUnits::ControlUnits(IWorld* world, const GSvector3& position) :
	units_{ MakeNumber },
	enemys_{ MakeNumber } {

	world_ = world;

	tag_ = "ControlUnitsTag";

	name_ = "ControlUnits";


	collider_ = BoundingSphere{ Radius,GSvector3{0.f,Height,0.f} };

	transform_.position(position);

	player_ = static_cast<Player*>(world_->find_actor("Player"));

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
}

void ControlUnits::update(float delta_time) {

	//自身の座標をプレイヤーの座標にする
	transform_.position(player_->transform().position());

	//自身の座標を更新
	pos = transform_.position();

	if (!Change) {
		if (StateNow(AllRangeUnit::State::Attack)) {
			settarget();
		}
	}
	else {
		retreat();
	}
	//敵配列の状態更新
	Enemyarraymanagement();
}

void ControlUnits::draw() const {

	gsTextPos(100, 600);
	gsDrawText("配列%d", test);

	collider().draw();
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
				test++;
				break;
			}
		}
	}
}

//生成時に対象を割り当てる
void ControlUnits::settarget() {

	for (auto& unit : units_) {
		//ターゲットをまだ所持していたら更新しない
		if (unit->retuntarget() != NULL)continue;

		Actor* target = PickTarget();

		if (target == NULL)return;

		//敵配列内のランダムな奴とそのタグを渡す
		unit->settarget(target, target->tag());
	}
}

//撤収
void ControlUnits::retreat() {
	for (auto& unit : units_) {
		unit->changestate(AllRangeUnit::State::Retreat);
	}
}

//全ユニットが指定したステータスに移行したかどうか
bool ControlUnits::StateNow(AllRangeUnit::State state) {

	int count = 0;

	for (auto& unit : units_) {

		if (unit->nowstate() == state) {
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

		float dis = GSvector3::distance(enemypos, pos);

		//保持している個体が死んだもしくは一定の距離以上離れたら除外
		if (enemy->tag() == "DieEnemyTag" ||
			dis > Radius) {
			enemy = NULL;
			test--;
		}
	}
}

//ターゲットを渡す
Actor* ControlUnits::PickTarget() {

	Actor* target = enemys_[gsRand(0, 4)];

	if (target != NULL) {
		return target;
	}

	sarchcounter++;

	if (sarchcounter > 5) {
		sarchcounter = 0;
		return NULL;
	}

	return PickTarget();
}

//撤退するかどうかのフラグ
void ControlUnits::changeFrag(bool frag) {
	Change = frag;
}