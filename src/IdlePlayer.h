/*
 * IdlePlayer.h
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#ifndef IDLEPLAYER_H_
#define IDLEPLAYER_H_

#include "Game.h"

class IdlePlayer : public Player {
public:
	IdlePlayer();
	virtual ~IdlePlayer();

	void gameStarts(int yourId, const Game::State &state);
	void playerDecisionMade(int playerId, const Game::Decision &decision, const Game::State &newState);

	struct Game::Decision makeDecision(const Game::State &state);
};

#endif /* IDLEPLAYER_H_ */
