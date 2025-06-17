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
	beamRifleCounter_ = 0;
	beamMagnumeCounter_ = 0;
	bazookaCounter_ = 0;
	allRangeUnitCounter_ = 0;
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

int GameData::beamRifleKillCounter() const {
	return beamRifleCounter_;
}

void GameData::setBeamRifleKillCounter(int counter) {
	beamRifleCounter_ += counter;
}

int GameData::beamMagnumKillCounter() const {
	return beamMagnumeCounter_;
}

void GameData::setBeamMagnumKillCounter(int counter) {
	beamMagnumeCounter_ += counter;
}

int GameData::bazookaKillCounter() const {
	return bazookaCounter_;
}

void GameData::setBazookaKillCounter(int counter) {
	bazookaCounter_ += counter;
}

int GameData::allRangeUnitKillCounter() const {
	return allRangeUnitCounter_;
}

void GameData::setAllRangeUnitKillCounter(int counter) {
	allRangeUnitCounter_ += counter;
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

void GameData::setIncomplete(bool frag) {

}

bool GameData::drawcollider() const
{
	return drawColliderFrag_;
}

void GameData::setdrawcollider(bool frag) {
	drawColliderFrag_ = frag;
}
