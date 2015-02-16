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

PlayerDecision::PlayerDecision(Move *move) {
	this->type = PlayerDecision::MOVE;
	this->data.move = move;
}

Game::Game(std::vector<Player *> &players) {
	this->players = players;
	this->currentState = NULL;
}

void Game::initializeState() {
	Board board = this->getInitialBoard();
	std::set<Tile *> bag = this->getInitialBag();
	this->currentState = new GameState(*this, board, bag);
}

GameState *Game::getCurrentState() {
	return this->currentState;
}

Game::~Game() {
	for (std::vector<GameState *>::iterator it = stateHistory.begin(); it != stateHistory.end(); it++) {
		delete &(*it);
	}
	delete this->currentState;
}

int Game::getPlayerCount() const {
	return players.size();
}

void Game::oneTurn() {
	int turn = currentState->getTurn();
	Player *currentPlayer = players[turn];
	PlayerState playerState(*currentState);

	struct PlayerDecision decision = currentPlayer->makeDecision(playerState);
	this->applyDecision(decision);

	GameState *newState = currentState; /* currentState is changed by applyDecision */
	PlayerState newPlayerState(*newState);
	for (int i = 0; i < players.size(); i++) {
		players[i]->playerDecisionMade(turn, decision, newPlayerState);
	}
}

void Game::applyDecision(PlayerDecision &decision) {
	int turn = currentState->getTurn();
	GameState *newState = new GameState(*currentState);
	Player *currentPlayer = players[turn];

	newState->applyDecision(decision);
	newState->advanceTurn();

	decisionHistory.push_back(decision);
	stateHistory.push_back(currentState);

	currentState = newState;
}

void Game::play() {
	initializeState();
	PlayerState playerState(*currentState);
	for (int i = 0; i < players.size(); i++) {
		players[i]->gameStarts(i, playerState);
	}

	while (!currentState->isFinal()) {
		oneTurn();
	}
}

Player::~Player() {};
