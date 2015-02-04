/*
 * Game.cpp
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */
#include <algorithm>
#include "Game.h"

Game::Decision::Decision(enum Decision::Type type, union Decision::Data data) {
	this->type = type;
	this->data = data;
}

Game::Decision::Decision(Move *move) {
	this->type = Game::Decision::MOVE;
	this->data.move = move;
}

Game::State::State(Game &game, Board &board, std::set<Tile *> &bag) :
		  racks(game.getPlayerCount()),
		  scores(game.getPlayerCount(), 0),
		  board(board) {
	this->game = &game;
	this->bag = bag;
	this->turn = 0;
}

Game::State::State(const Game::State &state) : game(state.game), board(state.board) {
	*this = state;
}

Game::State &Game::State::operator=(const Game::State &state) {
	this->game = state.game;
	this->board = state.board;
	this->bag = state.bag;
	this->turn = state.turn;
	this->racks = state.racks;
	this->scores = state.scores;
	return *this;
}

const std::set<Tile *>& Game::State::getRack(int playerId) const {
	return racks[playerId];
}

void Game::State::setRack(int playerId, const std::set<Tile *> &tiles) {
	for (std::set<Tile *>::iterator it = racks[playerId].begin(); it != racks[playerId].end(); it++) {
		bag.insert(*it);
	}
	for (std::set<Tile *>::iterator it = tiles.begin(); it != tiles.end(); it++) {
		bag.erase(*it);
		racks[playerId].insert(*it);
	}
	returnHandToBag();
}

void Game::State::repopulateRack() {
	std::set<Tile *> &rack = racks[turn];
	std::vector<Tile *> shuffledBag;
	for (std::set<Tile *>::iterator it = rack.begin(); it != rack.end(); it++) {
		shuffledBag.push_back(*it);
	}
	random_shuffle(shuffledBag.begin(), shuffledBag.end());

	while (!shuffledBag.empty() && rack.size() < game->getRackSize()) {
		Tile *tile = shuffledBag.back();
		rack.insert(tile);
		shuffledBag.pop_back();
	}
	returnHandToBag();
}

void Game::State::applyDecision(const Decision &decision) {
	switch (decision.type) {
		case Decision::MOVE: {
			int score = this->board.getMoveScore(*decision.data.move);
			this->board.applyMove(*decision.data.move);
			break;
		}
		case Decision::EXCHANGE: {
			const std::vector<Tile *> &tiles = *decision.data.exchangedTiles;
			for (std::vector<Tile *>::const_iterator it = tiles.begin(); it != tiles.end(); it++) {
				this->racks[this->turn].erase(*it);
				this->hand.push_back(*it);
			}
			break;
		}
	}
}

void Game::State::advanceTurn() {
	this->turn = (this->turn + 1) % game->players.size();
}

void Game::State::returnHandToBag() {
	bag.insert(hand.begin(), hand.end());
	hand.clear();
}

const Board &Game::State::getBoard() const {
	return board;
}

const std::set<Tile *>& Game::State::getRack() const {
	return racks[turn];
}

const std::vector<int>& Game::State::getScores() const {
	return scores;
}

int Game::State::getPlayerCount() const {
	return game->getPlayerCount();
}

bool Game::State::isFinal() const {
	if (!bag.empty()) {
		return false;
	} else {
		for (std::vector<std::set<Tile *> >::const_iterator it = racks.begin(); it != racks.end(); it++) {
			if (it->empty()) {
				return true;
			}
		}
		return false;
	}
}

Game::Game(std::vector<Player *> &players) {
	this->players = players;
	this->currentState = NULL;
}

void Game::initializeState() {
	Board board = this->getInitialBoard();
	std::set<Tile *> bag = this->getInitialBag();
	this->currentState = new Game::State(*this, board, bag);
}

Game::State *Game::getCurrentState() {
	return this->currentState;
}

Game::~Game() {
	for (std::vector<State *>::iterator it = stateHistory.begin(); it != stateHistory.end(); it++) {
		delete &(*it);
	}
}

int Game::getPlayerCount() const {
	return players.size();
}

void Game::oneStep() {
	int turn = currentState->turn;
	State *newState = new State(*currentState);
	Player *currentPlayer = players[turn];

	struct Decision decision = currentPlayer->makeDecision(*currentState);
	newState->applyDecision(decision);
	newState->repopulateRack();
	newState->advanceTurn();

	for (int i = 0; i < players.size(); i++) {
		players[i]->playerDecisionMade(turn, decision, *newState);
	}

	decisionHistory.push_back(decision);
	stateHistory.push_back(currentState);

	currentState = newState;
}

void Game::play() {
	initializeState();
	for (int i = 0; i < players.size(); i++) {
		players[i]->gameStarts(i, *currentState);
	}

	while (!currentState->isFinal()) {
		oneStep();
	}
}

Player::~Player() {};
