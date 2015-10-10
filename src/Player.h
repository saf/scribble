/*
 * Player.h
 *
 *  Created on: 18-01-2015
 *      Author: saf
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <memory>

class Decision;
class PlayerState;

class Player {
public:
	virtual ~Player();

	virtual void gameStarts(int playerId, const PlayerState &state) = 0;
	virtual void playerDecisionMade(int playerId, const Decision &decision, const PlayerState &newState) = 0;

	virtual std::shared_ptr<Decision> makeDecision(const PlayerState &state) = 0;
};

#endif /* PLAYER_H_ */
