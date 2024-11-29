#include "GameData.h"
#include <gslib.h>

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
bool GameData::bossRetreat() const{

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
