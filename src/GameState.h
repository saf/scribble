/*
 * GameState.h
 *
 *  Created on: 15-02-2015
 *      Author: saf
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "Game.h"

class Game;
class PlayerDecision;

class GameState {
	protected:
		Game *game;
		int turn; /* cycles through 0 ... (playerCount-1) */
		Board board;
		std::set<Tile *> bag;
		std::vector<std::set<Tile *> > racks;
		std::vector<int> scores;

		std::vector<Tile *> hand; /* Used for tiles exchanged out by a player. */
		void returnHandToBag();

	public:
		GameState(Game &game, Board &board, std::set<Tile *> &bag);
		GameState(const GameState &state);
		GameState& operator=(const GameState &state);
		virtual ~GameState() {};

		int getTurn() const;
		void advanceTurn();

		std::set<Tile *>& getBag();
		const std::set<Tile *>& getBag() const;
		std::vector<std::set<Tile *> >& getRacks();
		const std::vector<std::set<Tile *> >& getRacks() const;
		std::vector<int>& getScores();
		const std::vector<int>& getScores() const;
		Board& getBoard();
		const Board& getBoard() const;

		Game *getGame() const;

		bool isFinal() const;

		void repopulateRack(int playerId);
		void repopulateRack(int playerId, const std::vector<Tile *>& tiles);

		void applyDecision(const PlayerDecision &decision);
	};

class PlayerState : private GameState {
	private:

	public:
		PlayerState(const GameState &state);

		const Board &getBoard();
		const std::set<Tile *>& getRack();
		const std::vector<int>& getScores();
		int getPlayerCount();

		void applyDecision(const PlayerDecision &decision);
};

#endif /* GAMESTATE_H_ */
