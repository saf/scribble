/*
 * Game.cpp
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */
#include <algorithm>

#include "Game.h"
#include "GameState.h"

Game::Game(std::vector<std::unique_ptr<Player>> players)
		: players(std::move(players)) {
}

Game::Game(Game&& other)
		: players(std::move(other.players)),
		  currentState(std::move(other.currentState)),
		  stateHistory(std::move(other.stateHistory)),
		  decisionHistory(std::move(other.decisionHistory)) {
}

Game& Game::operator=(Game&& other) {
	std::swap(players, other.players);
	std::swap(currentState, other.currentState);
	std::swap(stateHistory, other.stateHistory);
	std::swap(decisionHistory, other.decisionHistory);
	return *this;
}

void Game::initializeState() {
	Board board = getInitialBoard();
	Bag bag = getInitialBag();
	currentState.reset(new GameState(*this, board, bag));
}

const GameState& Game::getCurrentState() {
	return *currentState;
}

int Game::getPlayerCount() const {
	return players.size();
}

void Game::oneTurn() {
	int turn = currentState->getTurn();
	Player& currentPlayer = *players[turn];
	PlayerState playerState(currentState, turn);

	std::shared_ptr<Decision> decision = currentPlayer.makeDecision(playerState);
	applyDecision(decision);

	for (size_t i = 0; i < players.size(); i++) {
		players[i]->playerDecisionMade(turn, *decision, PlayerState(currentState, i));
	}
}

void Game::applyDecision(const std::shared_ptr<Decision>& decision) {
	auto newState = currentState->stateAfterDecision(*decision);
	newState->advanceTurn();

	decisionHistory.emplace_back(decision);
	stateHistory.emplace_back(std::move(currentState));

	currentState = std::move(newState);
}

void Game::play() {
	initializeState();
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->gameStarts(i, PlayerState(currentState, i));
	}

	while (!currentState->isFinal()) {
		oneTurn();
	}
}

Player::~Player() {};
