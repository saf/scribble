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

void IdlePlayer::gameStarts(int yourId, const PlayerState &state) {}
void IdlePlayer::playerDecisionMade(int playerId, const PlayerDecision &decision, const PlayerState &newState) {}

struct PlayerDecision IdlePlayer::makeDecision(const PlayerState &state) {
	return PlayerDecision(PlayerDecision::PASS, PlayerDecision::Data());
}
