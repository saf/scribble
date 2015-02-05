/*
 * IdlePlayer.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include "IdlePlayer.h"

IdlePlayer::IdlePlayer() {
	// TODO Auto-generated constructor stub
}

IdlePlayer::~IdlePlayer() {
	// TODO Auto-generated destructor stub
}

void IdlePlayer::gameStarts(int yourId, const Game::State &state) {}
void IdlePlayer::playerDecisionMade(int playerId, const Game::Decision &decision, const Game::State &newState) {}

struct Game::Decision IdlePlayer::makeDecision(const Game::State &state) {
	return Game::Decision(Game::Decision::PASS, Game::Decision::Data());
}
