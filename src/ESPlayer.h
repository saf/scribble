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
#include "ESDictionary.h"

class ESPlayer : public Player {
private:
	ESDictionary &dictionary;

	std::vector<ESHook> getHooks(const Board& board);
public:
	ESPlayer(ESDictionary &dict);
	virtual ~ESPlayer();

	virtual void gameStarts(int yourId, const PlayerState& state);
	virtual void playerDecisionMade(int playerId, const Decision& decision, const PlayerState& newState);

	virtual std::shared_ptr<Decision> makeDecision(const PlayerState& state);
};

#endif /* ESPLAYER_H_ */
