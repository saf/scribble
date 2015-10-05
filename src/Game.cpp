/*
 * Game.cpp
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */
#include <algorithm>
#include "Game.h"

PlayerDecision::PlayerDecision(enum PlayerDecision::Type type, union PlayerDecision::Data data) {
	this->type = type;
	this->data = data;
}

PlayerDecision::PlayerDecision(Move *move)
		: type(PlayerDecision::MOVE) {
	this->data.move = move;
}

Game::Game(std::vector<Player *> &players)
		: players(players) {
}

void Game::initializeState() {
	Board board = getInitialBoard();
	std::set<Tile *> bag = getInitialBag();
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
	Player *currentPlayer = players[turn];
	PlayerState playerState(currentState, turn);

	struct PlayerDecision decision = currentPlayer->makeDecision(playerState);
	applyDecision(decision);

	for (size_t i = 0; i < players.size(); i++) {
		players[i]->playerDecisionMade(turn, decision, PlayerState(currentState, i));
	}
}

void Game::applyDecision(PlayerDecision &decision) {
	auto newState = currentState->stateAfterDecision(decision);
	newState->advanceTurn();

	decisionHistory.push_back(decision);
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
