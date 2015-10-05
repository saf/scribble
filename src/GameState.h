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
	public:
		GameState(Game &game, Board &board, std::set<Tile *> &bag);
		GameState(const GameState &state);

		GameState& operator=(const GameState &other) = delete;

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

		const Game& getGame() const;

		bool isFinal() const;

		void repopulateRack(int playerId);
		void repopulateRack(int playerId, const std::vector<Tile *>& tiles);

		std::shared_ptr<GameState> stateAfterDecision(const PlayerDecision &decision) const;

	protected:
		const Game& game;
		int turn; /* cycles through 0 ... (playerCount-1) */
		Board board;
		std::set<Tile *> bag;
		std::vector<std::set<Tile *> > racks;
		std::vector<int> scores;

		std::vector<Tile *> hand; /* Used for tiles exchanged out by a player. */
		void returnHandToBag();

	};

class PlayerState {
	public:
		PlayerState(std::shared_ptr<GameState> state, int playerId_);

		const Board &getBoard() const;
		const std::set<Tile *>& getRack() const;
		const std::vector<int>& getScores() const;
		int getPlayerCount() const;

		PlayerState applyDecision(const PlayerDecision &decision);
	private:
		const std::shared_ptr<GameState> state_;
		int playerId_;
};

#endif /* GAMESTATE_H_ */
