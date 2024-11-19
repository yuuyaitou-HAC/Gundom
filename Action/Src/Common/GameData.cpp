#include "GameData.h"
#include <gslib.h>

int GameData::dieEnemyCounter()const {

	return EnemyKillCounter_;

}

void GameData::setDieEnemyCounter(int counter) {

	EnemyKillCounter_ += counter;

}


bool GameData::bossDie() const{

	return BossDieFrag_;
}

void GameData::setBossDie(bool frag) {

	BossDieFrag_ = frag;

}

bool GameData::playerDie() const{

	return PlayerDieFrag_;
}

void GameData::setPlayerDie(bool frag) {

	PlayerDieFrag_ = frag;

}
