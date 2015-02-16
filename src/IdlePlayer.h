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

	void gameStarts(int yourId, const PlayerState &state);
	void playerDecisionMade(int playerId, const PlayerDecision &decision, const PlayerState &newState);

	struct PlayerDecision makeDecision(const PlayerState &state);
};

#endif /* IDLEPLAYER_H_ */
