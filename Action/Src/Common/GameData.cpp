#include "GameData.h"
#include <gslib.h>

//初期化
void GameData::initialize() {

	enemyKillCounter_ = 0;
	underBossMakeFrag_ = false;
	underBossDieFrag_ = false;
	bossDieFrag_ = false;
	bossMakeFrag_ = false;
	playerDieFrag_ = false;
	underBossRetreatFrag_ = true;
	playerSupplyFrag_ = false;
	gameClearFrag_ = false;
	missionCounter_ = 0;
	beamSaberecounter_ = 0;
	beamRiflecounter_ = 0;
	beamMagnumecounter_ = 0;
	bazookaecounter_ = 0;
	allRangeUnitecounter_ = 0;
}

//死んだ敵の数を返す
int GameData::dieEnemyCounter()const {
	return enemyKillCounter_;
}

//死んだ敵の数を変更
void GameData::setDieEnemyCounter(int counter) {
	enemyKillCounter_ += counter;
}

//ボス生成フラグを返す
bool GameData::underBossMake() const {
	return underBossMakeFrag_;
}

//ボス生成フラグの変更
void GameData::setUnderBossMake(bool frag) {
	underBossMakeFrag_ = frag;
}


bool GameData::bossMake() const
{
	return bossMakeFrag_;
}

void GameData::setBossMake(bool frag) {
	bossMakeFrag_ = frag;
}

//ボスの退却を返す
bool GameData::bossRetreat() const {
	return underBossRetreatFrag_;
}

//ボスの退却を決める
void GameData::setBossRetreat(bool frag) {
	underBossRetreatFrag_ = frag;
}

//中ボスの生死を返す
bool GameData::underBossDie() const {
	return underBossDieFrag_;
}

//中ボスの生死を決める
void GameData::setUnderBossDie(bool frag) {
	underBossDieFrag_ = frag;
}

//ボスの生死を返す
bool GameData::bossDie() const {
	return bossDieFrag_;
}

//ボスの生死を決める
void GameData::setBossDie(bool frag) {
	bossDieFrag_ = frag;
}

//プレイヤーの生死を返す
bool GameData::playerDie() const {
	return playerDieFrag_;
}

//プレイヤーの生死を変える
void GameData::setPlayerDie(bool frag) {
	playerDieFrag_ = frag;
}

//プレイヤーの補給状況を返す
bool GameData::playerSupply() const {
	return playerSupplyFrag_;
}

//プレイヤーが補給中かどうかを返す
void GameData::setPlayerSupply(bool frag) {
	playerSupplyFrag_ = frag;
}

bool GameData::gameClearFrag() const {
	return gameClearFrag_;
}

void GameData::setGameClearFrag(bool frag) {
	gameClearFrag_ = frag;
}

int GameData::missionClear() const {
	return missionCounter_;
}

void GameData::setMissionClear(int counter) {
	missionCounter_ = counter;
}

int GameData::beamSaberKillCounter() const {
	return beamSaberecounter_;
}

void GameData::setBeamSaberKillCounter(int counter) {
	beamSaberecounter_ += counter;
}

int GameData::beamRifleKillCounter() const {
	return beamRiflecounter_;
}

void GameData::setBeamRifleKillCounter(int counter) {
	beamRiflecounter_ += counter;
}

int GameData::beamMagnumKillCounter() const {
	return beamMagnumecounter_;
}

void GameData::setBeamMagnumKillCounter(int counter) {
	beamMagnumecounter_ += counter;
}

int GameData::bazookaKillCounter() const {
	return bazookaecounter_;
}

void GameData::setBazookaKillCounter(int counter) {
	bazookaecounter_ += counter;
}

int GameData::allRangeUnitKillCounter() const {
	return allRangeUnitecounter_;
}

void GameData::setAllRangeUnitKillCounter(int counter) {
	allRangeUnitecounter_ += counter;
}

bool GameData::withdrawalInstructions() const
{
	return retreatFrag_;
}

void GameData::setWithdrawalInstructions(bool frag)
{
	retreatFrag_ = frag;
}

bool GameData::incomplete() const
{
	return false;
}

void GameData::setIncomplete(bool frag)
{
}
