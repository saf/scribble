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
class Decision;
class MoveDecision;
class TileExchangeDecision;
class PassDecision;

class GameState {
	public:
		GameState(Game& game, Board& board, Tileset& bag);
		GameState(const GameState &state);

		GameState& operator=(const GameState &other) = delete;

		virtual ~GameState() {};

		int getTurn() const;
		void advanceTurn();

		Bag& getBag();
		const Bag& getBag() const;

		std::vector<Rack>& getRacks();
		const std::vector<Rack>& getRacks() const;

		std::vector<int>& getScores();
		const std::vector<int>& getScores() const;

		Board& getBoard();
		const Board& getBoard() const;

		const Game& getGame() const;

		bool isFinal() const;

		void repopulateRack(int playerId);
		void repopulateRack(int playerId, const Tileset& tiles);

		std::shared_ptr<GameState> stateAfterDecision(const Decision& decision) const;

		void applyMoveDecision(const MoveDecision& decision);
		void applyExchangeDecision(const TileExchangeDecision& decision);
		void applyPassDecision(const PassDecision& decision);

	protected:
		const Game& game;
		int turn; /* cycles through 0 ... (playerCount-1) */
		Board board;
		Bag bag;
		std::vector<Rack> racks;
		std::vector<int> scores;

		Tileset hand; /* Used for tiles exchanged out by a player. */
		void returnHandToBag();
	};

class PlayerState {
	public:
		PlayerState(std::shared_ptr<GameState> state, int playerId_);

		const Board &getBoard() const;
		const Rack& getRack() const;
		const std::vector<int>& getScores() const;
		int getPlayerCount() const;

		PlayerState applyDecision(const Decision &decision);
	private:
		const std::shared_ptr<GameState> state_;
		int playerId_;
};

#endif /* GAMESTATE_H_ */
