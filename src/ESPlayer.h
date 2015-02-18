/*
 * ESPlayer.h
 *
 *  Created on: 16-02-2015
 *      Author: saf
 */

#ifndef ESPLAYER_H_
#define ESPLAYER_H_

#include "Game.h"
#include "Trie.h"

class ESPlayer : public Player {
private:
	enum HookDirection { LEFT, RIGHT, UP, DOWN };
	struct Hook {
		int row;
		int col;
		enum HookDirection direction;
	};

	std::vector<Hook> getHooks(const Board& board);
public:
	ESPlayer();
	virtual ~ESPlayer();

	virtual void gameStarts(int yourId, const PlayerState &state);
	virtual void playerDecisionMade(int playerId, const PlayerDecision &decision, const PlayerState &newState);

	virtual struct PlayerDecision makeDecision(const PlayerState &state);
};

#endif /* ESPLAYER_H_ */
