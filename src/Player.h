/*
 * Player.h
 *
 *  Created on: 18-01-2015
 *      Author: saf
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Game.h"

class Player {
public:
	virtual ~Player();

	virtual void gameStarts(int yourId, const Game::State &state) = 0;
	virtual void playerDecisionMade(int playerId, const Game::Decision &decision, const Game::State &newState) = 0;

	virtual void makeDecision(const Game::State &state) = 0;
};

#endif /* PLAYER_H_ */
