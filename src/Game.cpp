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

Game::State::State(Game &game, Board &board, std::set<Tile *> &bag) {
	racks(game.getPlayerCount());
	scores(game.getPlayerCount(), 0);
	this->game = game;
	this->board = board;
	this->bag = bag;
	this->turn = 0;
}

Game::State::State(const Game::State &state) {
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
}

void Game::State::repopulateRack(int playerId) {
	std::vector<Tile *> shuffledBag(racks[playerId]);
	random_shuffle(shuffledBag.begin(), shuffledBag.end());

	while (!shuffledBag.empty() && racks[playerId].size() < game.getRackSize()) {
		Tile *tile = shuffledBag.pop_back();
		racks[playerId].insert(tile);
	}
}

void Game::State::applyDecision(const Decision &decision) {
	switch (decision.type) {
	case Decision::MOVE:
		int score = this->board.getMoveScore(*decision.data.move);
		this->board.applyMove(*decision.data.move);
		break;
	case Decision::EXCHANGE:
		const std::vector<Tile *> &tiles = decision.data.exchangedTiles;
		for (std::vector<Tile *>::const_iterator it = tiles.begin(); it != tiles.end(); it++) {
			this->racks[this->turn].erase(*it);
		}
		break;
	}

	this->turn = (this->turn + 1) % game.players.size();
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
	return game.getPlayerCount();
}

bool Game::State::isFinal() const {
	if (!bag.empty()) {
		return false;
	} else {
		for (std::vector<std::set<Tile *> >::iterator it = racks.begin(); it != racks.end(); it++) {
			if (it->empty()) {
				return true;
			}
		}
		return false;
	}
}

Game::Game(int playerCount, std::vector<Player *> &players) {
	Board board = this->getInitialBoard();
	std::set<Tile *> bag = this->getInitialBag();
	this->players = players;
	this->currentState = new Game::State(*this, board, bag);
}

Game::~Game() {
}

int Game::getPlayerCount() const {
	return players.size();
}

void Game::oneStep() {
}
