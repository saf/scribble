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
		Game &game;
		int turn; /* cycles through 0 ... (playerCount-1) */
		Board board;
		std::set<Tile *> bag;
		std::vector<std::set<Tile *> > racks;
		std::vector<int> scores;

		const std::set<Tile *> &getRack(int playerId) const;
		void setRack(int playerId, const std::set<Tile *> &tiles);
		void repopulateRack(int playerId);

	public:
		State(Game &game, Board &board, std::set<Tile *> &bag);
		State(const State &state);
		State& operator=(const State &state);
		virtual ~State();

		void applyDecision(const Decision &decision);

		const Board& getBoard() const;
		const std::set<Tile *>& getRack() const ; /* Returns rack for current player */
		const std::vector<int>& getScores() const;
		int getPlayerCount() const;

		bool isFinal() const;
	};

private:
	std::vector<Player *> players;

	State *currentState;
	std::vector<State *> stateHistory;
	std::vector<Decision *> decisionHistory;

	virtual void getInitialBoard() = 0;
	virtual void getInitialBag() = 0;

public:
	Game(int playerCount, std::vector<Player *> &players);
	virtual ~Game();

	virtual void getRackSize() = 0;
	int getPlayerCount() const;

	void oneStep();
	void play();
};

#endif /* GAME_H_ */
