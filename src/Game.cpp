/*
 * Game.cpp
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */
#include <algorithm>
#include <assert.h>

#include "Game.h"
#include "GameState.h"

Game::Game(std::vector<std::unique_ptr<Player>> players)
		: players_(std::move(players)) {
}

Game::Game(Game&& other)
		: players_(std::move(other.players_)),
		  currentState_(std::move(other.currentState_)),
		  stateHistory_(std::move(other.stateHistory_)),
		  decisionHistory_(std::move(other.decisionHistory_)) {
}

Game& Game::operator=(Game&& other) {
	std::swap(players_, other.players_);
	std::swap(currentState_, other.currentState_);
	std::swap(stateHistory_, other.stateHistory_);
	std::swap(decisionHistory_, other.decisionHistory_);
	return *this;
}

void Game::initializeState() {
	board_ = getInitialBoard();
	currentState_.reset(new GameState(*this, getInitialBag()));
}

const GameState& Game::getCurrentState() {
	return *currentState_;
}

const std::deque<std::shared_ptr<GameState>>& Game::getStateHistory() const {
	return stateHistory_;
}

const Board& Game::getBoard() const {
	assert(board_);
	return *board_;
}

int Game::getPlayerCount() const {
	return players_.size();
}

void Game::oneTurn() {
	int turn = currentState_->getTurn();
	Player& currentPlayer = *players_[turn];
	PlayerState playerState(currentState_, turn);

	std::shared_ptr<Decision> decision = currentPlayer.makeDecision(playerState);
	applyDecision(decision);

	for (size_t i = 0; i < players_.size(); i++) {
		players_[i]->playerDecisionMade(turn, *decision, PlayerState(currentState_, i));
	}
}

void Game::applyDecision(const std::shared_ptr<Decision>& decision) {
	auto newState = currentState_->stateAfterDecision(*decision);
	newState->advanceTurn();

	decisionHistory_.emplace_back(decision);
	stateHistory_.emplace_back(std::move(currentState_));

	currentState_ = std::move(newState);
}

void Game::play() {
	initializeState();
	for (size_t i = 0; i < players_.size(); i++) {
		players_[i]->gameStarts(i, PlayerState(currentState_, i));
	}

	while (!currentState_->isFinal()) {
		oneTurn();
	}
}

Player::~Player() {};
