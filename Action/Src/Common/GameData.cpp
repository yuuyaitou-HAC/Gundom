#include "GameData.h"
#include <gslib.h>

void GameData::initialize() {

	EnemyKillCounter_ = 0;
	BossMakeFrag_ = false;
	BossDieFrag_ = false;;
	PlayerDieFrag_ = false;;
	BossRetreatFrag_ = false;;
	PlayerSupplyFrag_ = false;;
	GameClearFrag_ = false;;
	MissionCounter_ = 0;
	BeamSaberecounter_ = 0;
	BeamRiflecounter_ = 0;
	BeamMagnumecounter_ = 0;
	Bazookaecounter_ = 0;
	AllRangeUnitecounter_ = 0;
}

//死んだ敵の数を返す
int GameData::dieEnemyCounter()const {
	return EnemyKillCounter_;
}

//死んだ敵の数を変更
void GameData::setDieEnemyCounter(int counter) {
	EnemyKillCounter_ += counter;
}

//ボス生成フラグを返す
bool GameData::bossMake() const {
	return BossMakeFrag_;
}

//ボス生成フラグの変更
void GameData::setBossMake(bool frag) {
	BossMakeFrag_ = frag;
}

//ボスの退却を返す
bool GameData::bossRetreat() const {
	return BossRetreatFrag_;
}

//ボスの退却を決める
void GameData::setBossRetreat(bool frag) {
	BossRetreatFrag_ = frag;
}

//ボスの生死を返す
bool GameData::bossDie() const {
	return BossDieFrag_;
}

//ボスの生死を決める
void GameData::setBossDie(bool frag) {
	BossDieFrag_ = frag;
}

//プレイヤーの生死を返す
bool GameData::playerDie() const {
	return PlayerDieFrag_;
}

//プレイヤーの生死を変える
void GameData::setPlayerDie(bool frag) {
	PlayerDieFrag_ = frag;
}

//プレイヤーの補給状況を返す
bool GameData::playerSupply() const {
	return PlayerSupplyFrag_;
}

//プレイヤーが補給中かどうかを返す
void GameData::setPlayerSupply(bool frag) {
	PlayerSupplyFrag_ = frag;
}

bool GameData::gameClearFrag() const {
	return GameClearFrag_;
}

void GameData::setGameClearFrag(bool frag) {
	GameClearFrag_ = frag;
}

int GameData::missionClear() const {
	return MissionCounter_;
}

void GameData::setMissionClear(int counter) {
	MissionCounter_ = counter;
}

int GameData::beamSaberKillCounter() const {
	return BeamSaberecounter_;
}

void GameData::setBeamSaberKillCounter(int counter) {
	BeamSaberecounter_ += counter;
}

int GameData::beamRifleKillCounter() const {
	return BeamRiflecounter_;
}

void GameData::setBeamRifleKillCounter(int counter) {
	BeamRiflecounter_ += counter;
}

int GameData::beamMagnumKillCounter() const {
	return BeamMagnumecounter_;
}

void GameData::setBeamMagnumKillCounter(int counter) {
	BeamMagnumecounter_ += counter;
}

int GameData::BazookaKillCounter() const {
	return Bazookaecounter_;
}

void GameData::setBazookaKillCounter(int counter) {
	Bazookaecounter_ += counter;
}

int GameData::AllRangeUnitKillCounter() const {
	return AllRangeUnitecounter_;
}

void GameData::setAllRangeUnitKillCounter(int counter) {
	AllRangeUnitecounter_ += counter;
}