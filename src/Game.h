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

class Player;

class Game {
public:
	struct Decision {
	public:
		enum Type { MOVE, EXCHANGE, PASS };
		union Data {
			Move *move;                          /* for MOVE decisions */
			std::vector<Tile *> *exchangedTiles; /* for EXCHANGE decisions */
		};

		Type type;
		Data data;

		Decision(enum Type type, union Data data);
		Decision(Move *move);
	};

	class State {
	private:
		Game *game;
		int turn; /* cycles through 0 ... (playerCount-1) */
		Board board;
		std::set<Tile *> bag;
		std::vector<std::set<Tile *> > racks;
		std::vector<int> scores;

		/* This collection is used for tiles which are being exchanged */
		std::vector<Tile *> hand;

		const std::set<Tile *> &getRack(int playerId) const;
		void setRack(int playerId, const std::set<Tile *> &tiles);

		/* Repopulate the current player's rack randomly with tiles from the bag. */
		void repopulateRack();

		/* After getting new tiles when exchanging, bring the exchanged tiles back into the bag */
		void returnHandToBag();

		void advanceTurn();

	public:
		State(Game &game, Board &board, std::set<Tile *> &bag);
		State(const State &state);
		State& operator=(const State &state);
		virtual ~State() {};

		void applyDecision(const Decision &decision);

		const Board& getBoard() const;
		const std::set<Tile *>& getRack() const ; /* Returns rack for current player */
		const std::vector<int>& getScores() const;
		int getPlayerCount() const;

		friend class Game;

		bool isFinal() const;
	};

private:
	std::vector<Player *> players;

	State *currentState;
	std::vector<State *> stateHistory;
	std::vector<Decision> decisionHistory;

protected:
	virtual Board getInitialBoard() = 0;
	virtual std::set<Tile *> getInitialBag() = 0;
	virtual void initializeState();

public:
	Game(std::vector<Player *> &players);
	virtual ~Game();

	virtual int getRackSize() = 0;
	int getPlayerCount() const;

	virtual void oneStep();
	virtual void play();
};

class Player {
public:
	virtual ~Player();

	virtual void gameStarts(int yourId, const Game::State &state) = 0;
	virtual void playerDecisionMade(int playerId, const Game::Decision &decision, const Game::State &newState) = 0;

	virtual struct Game::Decision makeDecision(const Game::State &state) = 0;
};

#endif /* GAME_H_ */
