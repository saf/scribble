/*
 * Game.h
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */

#ifndef GAME_H_
#define GAME_H_

#include <deque>
#include <list>
#include <memory>
#include <set>
#include <vector>

#include "Board.h"
#include "Basics.h"
#include "Move.h"
#include "Player.h"
#include "TilePlacement.h"

class Decision;

class GameState;
class PlayerState;

class Game {
public:
	Game(std::vector<std::unique_ptr<Player>> players);
	virtual ~Game() {};

	Game(const Game&) = delete;
	Game(Game&&);

	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&);

	virtual int getRackSize() const = 0;

	const Board& getBoard() const;

	int getPlayerCount() const;

	virtual void initializeState();

	const GameState& getCurrentState();
	const std::deque<std::shared_ptr<GameState>>& getStateHistory() const;

	virtual void applyDecision(const std::shared_ptr<Decision>& decision);
	virtual void oneTurn();
	virtual void play();

	virtual int score(const TilePlacement& currentTiles, const Move& move) const = 0;

protected:
	std::unique_ptr<Board> board_;

	std::vector<std::unique_ptr<Player>> players_;

	std::shared_ptr<GameState> currentState_;

	std::deque<std::shared_ptr<GameState>> stateHistory_;
	std::deque<std::shared_ptr<Decision>> decisionHistory_;

	virtual std::unique_ptr<Board> getInitialBoard() = 0;
	virtual Bag getInitialBag() = 0;
};

#endif /* GAME_H_ */
