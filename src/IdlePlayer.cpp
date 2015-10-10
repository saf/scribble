/*
 * IdlePlayer.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include "IdlePlayer.h"

#include "Decision.h"

IdlePlayer::IdlePlayer() {
}

void IdlePlayer::gameStarts(int yourId, const PlayerState &state) {}
void IdlePlayer::playerDecisionMade(int playerId, const Decision &decision, const PlayerState &newState) {}

struct std::shared_ptr<Decision> IdlePlayer::makeDecision(const PlayerState &state) {
	return std::shared_ptr<Decision>(new PassDecision());
}
