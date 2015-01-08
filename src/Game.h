/*
 * Game.h
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */

#ifndef GAME_H_
#define GAME_H_

#include <set>

class Game {
public:
	class Decision {
	private:
		enum Type { MOVE, EXCHANGE, PASS };
		Type type;
		union {
			Move *move;                          /* for MOVE decisions */
			std::vector<Tile *> *exchangedTiles; /* for EXCHANGE decisions */
		};

	public:
		Decision()
	};

	class State {
	private:
		int turn; /* cycles through 0, 1, (2, 3) */
		Board board;
		std::set<Tile *> bag;
		std::set<Tile *> *racks;
		int *scores;
	public:
		State(int playerCount, Board &board, std::set<Tile *> &bag);
		State(const State &state);
		State& operator=(const State &state);
		virtual ~State();

		void applyDecision(const Decision &decision);

		const Board& getBoard();
		const std::set<Tile *>& getBag();
		const std::set<Tile *>& getRack(); /* Returns rack for current player */
		const int *getScores();
	};

private:
	State currentState;
	std::vector<State *> stateHistory;
	std::vector<Decision *> decisionHistory;

public:
	Game(int playerCount);
	virtual ~Game();
};

#endif /* GAME_H_ */
