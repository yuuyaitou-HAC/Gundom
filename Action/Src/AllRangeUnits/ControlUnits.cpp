#include "ControlUnits.h"
#include "AllRangeUnit.h"
#include "Player/Player.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Collision/Line.h"

//ユニットの生成数
const int MakeNumber = 1;

ControlUnits::ControlUnits(IWorld* world, const GSvector3& position) :
	units_{ MakeNumber },
	enemys_{ MakeNumber } {

	world_ = world;

	tag_ = "ControlUnitsTag";

	name_ = "ControlUnits";

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


}

void ControlUnits::update(float delta_time) {

	if (!Change) {
		if (StateNow(AllRangeUnit::State::Attack)) {
			if (!SetFrag) {
				settarget();
			}
			else {
				retarget();
			}
		}
	}
	else {
		retreat();
	}
}

//生成時に対象を割り当てる
void ControlUnits::settarget() {

	//敵感知用の当たり判定を生成
	cd_ = new CollisionDerection{ world_,player_->transform().position(),"EnemyTag",10.0f };
	world_->add_actor(cd_);

	for (auto& unit : units_) {

		//対象を渡す
		unit->settarget(searchtaget());
	}

	cd_->die();

	SetFrag = true;

}

//対象がなくなっている固体を取得して再度割り当てる
void ControlUnits::retarget() {

}

//撤収
void ControlUnits::retreat() {
	for (auto& unit : units_) {
		unit->changestate(AllRangeUnit::State::Retreat);
	}
}

Actor* ControlUnits::searchtaget() {

	//当たった相手を取得
	enemy = cd_->actor();

	//取得した相手がかぶっていないか
	for (auto& target : enemys_) {

		if (enemy == target && enemy != NULL) {
			same = true;
		}
	}

	//被りがあったら再度取得しなおす
	if (same) {
		same = false;
		return searchtaget();
	}
	else {
		//被りが無かったら配列に格納して対象を返す
		for (auto& target : enemys_) {
			if (target == NULL) {
				target = enemy;
				break;
			}
		}
		return enemy;
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

void ControlUnits::changeFrag(bool frag) {
	Change = frag;
}