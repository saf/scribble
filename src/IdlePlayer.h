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

	void gameStarts(int yourId, const PlayerState &state) override;
	void playerDecisionMade(int playerId, const Decision &decision, const PlayerState &newState) override;

	struct std::shared_ptr<Decision> makeDecision(const PlayerState &state) override;
};

#endif /* IDLEPLAYER_H_ */
