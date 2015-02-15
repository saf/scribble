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

std::set<Tile *> Game::State::findTilesForPlayerRack(int playerId, const wchar_t *rackLetters) {
	const wchar_t *p = rackLetters;
	std::set<Tile *> empty;
	std::set<Tile *> rack;
	setRack(playerId, empty); /* Return to bag */

	while (*p) {
		wchar_t letter = *p;
		bool found = false;
		for (std::set<Tile *>::iterator it = bag.begin(); it != bag.end(); it++) {
			if ((*p == L'_' && (*it)->isBlank()) || (*it)->getLetter() == letter) {
				Tile *tile = *it;
				bag.erase(it);
				rack.insert(tile);
				found = true;
				break;
			}
		}
		if (!found) {
			throw "Failed to find tile for player rack.";
		}
		p++;
	}

	return rack;
}

std::vector<Tile *> Game::State::findTilesForPlayerMove(int playerId, int row, int column, Move::Direction direction, const wchar_t *wordLetters) {
	std::set<Tile *> rack = racks[playerId];
	std::vector<Tile *> moveTiles;
	const wchar_t *p = wordLetters;
	bool blankTile;

	while (*p) {
		wchar_t letter = *p;

		if (*p == L'[') {
			blankTile = true;
		} else if (*p != L']') {
			bool found = false;
			if (board.getTile(row, column) == NULL) {
				for (std::set<Tile *>::iterator it = rack.begin(); it != rack.end(); it++) {
					if (blankTile && (*it)->isBlank()) {
						BlankTile *blank = static_cast<BlankTile *>(*it);
						blank->fillLetter(letter);
						moveTiles.push_back(blank);
						found = true;
						break;
					} else if ((*it)->getLetter() == letter) {
						moveTiles.push_back(*it);
						found = true;
						break;
					}
				}
				if (!found) {
					throw "Failed to find tile for player move.";
				}
			}

			if (direction == Move::HORIZONTAL) {
				column++;
			} else {
				row++;
			}
			blankTile = false;
		}
		p++;
	}

	return moveTiles;
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
	delete this->currentState;
}

int Game::getPlayerCount() const {
	return players.size();
}

void Game::oneTurn() {
	int turn = currentState->turn;
	Player *currentPlayer = players[turn];

	struct Decision decision = currentPlayer->makeDecision(*currentState);
	this->applyDecision(decision);

	State *newState = currentState; /* currentState is changed by applyDecision */
	newState->repopulateRack();
	for (int i = 0; i < players.size(); i++) {
		players[i]->playerDecisionMade(turn, decision, *newState);
	}
}

void Game::applyDecision(Decision &decision) {
	int turn = currentState->turn;
	State *newState = new State(*currentState);
	Player *currentPlayer = players[turn];

	newState->applyDecision(decision);
	newState->advanceTurn();

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
		oneTurn();
	}
}

Player::~Player() {};
