/*
 * Game.h
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */

#ifndef GAME_H_
#define GAME_H_

#include <set>
#include <vector>
#include "Move.h"
#include "Board.h"
#include "GameState.h"

class Player;
class GameState;
class PlayerState;

struct PlayerDecision {
public:
	enum Type { MOVE, EXCHANGE, PASS };
	union Data {
		Move *move;                          /* for MOVE decisions */
		std::vector<Tile *> *exchangedTiles; /* for EXCHANGE decisions */
	};

	Type type;
	Data data;

	PlayerDecision(enum Type type, union Data data);
	PlayerDecision(Move *move);
};


class Game {
protected:
	std::vector<Player *> players;

	GameState *currentState;
	std::vector<GameState *> stateHistory;
	std::vector<PlayerDecision> decisionHistory;
	virtual Board getInitialBoard() = 0;
	virtual std::set<Tile *> getInitialBag() = 0;

public:
	Game(std::vector<Player *> &players);
	virtual ~Game();

	virtual int getRackSize() = 0;
	int getPlayerCount() const;

	virtual void initializeState();
	virtual GameState *getCurrentState();

	virtual void applyDecision(PlayerDecision &d);
	virtual void oneTurn();
	virtual void play();
};

class Player {
public:
	virtual ~Player();

	virtual void gameStarts(int yourId, const PlayerState &state) = 0;
	virtual void playerDecisionMade(int playerId, const PlayerDecision &decision, const PlayerState &newState) = 0;

	virtual struct PlayerDecision makeDecision(const PlayerState &state) = 0;
};

#endif /* GAME_H_ */
