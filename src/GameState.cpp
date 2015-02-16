/*
 * GameState.cpp
 *
 *  Created on: 15-02-2015
 *      Author: saf
 */

#include <algorithm>
#include "GameState.h"

GameState::GameState(Game &game, Board &board, std::set<Tile *> &bag) :
		  racks(game.getPlayerCount()),
		  scores(game.getPlayerCount(), 0),
		  board(board) {
	this->game = &game;
	this->bag = bag;
	this->turn = 0;
}

GameState::GameState(const GameState &state) : game(state.game), board(state.board) {
	*this = state;
}

GameState &GameState::operator=(const GameState &state) {
	this->game = state.game;
	this->board = state.board;
	this->bag = state.bag;
	this->turn = state.turn;
	this->racks = state.racks;
	this->scores = state.scores;
	return *this;
}

void GameState::repopulateRack(int playerId) {
	std::set<Tile *>& rack = racks[playerId];
	int count = game->getRackSize() - rack.size();
	std::vector<Tile *> tiles;

	if (count > 0) {
		std::vector<Tile *> shuffledBag;
		for (std::set<Tile *>::iterator it = bag.begin(); it != bag.end(); it++) {
			shuffledBag.push_back(*it);
		}
		random_shuffle(shuffledBag.begin(), shuffledBag.end());

		while (count) {
			Tile *tile = shuffledBag.back();
			tiles.push_back(tile);
			shuffledBag.pop_back();
		}
	}
	repopulateRack(playerId, tiles);
}

void GameState::repopulateRack(int playerId, const std::vector<Tile *>& tiles) {
	for (std::vector<Tile *>::const_iterator it = tiles.begin(); it != tiles.end(); it++) {
		bag.erase(*it);
		racks[playerId].insert(*it);
	}
}

void GameState::applyDecision(const PlayerDecision &decision) {
	switch (decision.type) {
		case PlayerDecision::MOVE: {
			int score = this->board.getMoveScore(*decision.data.move);
			this->board.applyMove(*decision.data.move);
			break;
		}
		case PlayerDecision::EXCHANGE: {
			const std::vector<Tile *> &tiles = *decision.data.exchangedTiles;
			for (std::vector<Tile *>::const_iterator it = tiles.begin(); it != tiles.end(); it++) {
				this->racks[this->turn].erase(*it);
				this->hand.push_back(*it);
			}
			break;
		}
	}
}

int GameState::getTurn() const {
	return turn;
}

void GameState::advanceTurn() {
	this->turn = (this->turn + 1) % game->getPlayerCount();
	returnHandToBag();
}

void GameState::returnHandToBag() {
	bag.insert(hand.begin(), hand.end());
	hand.clear();
}

std::set<Tile *>& GameState::getBag() {
	return bag;
}

const std::set<Tile *>& GameState::getBag() const {
	return bag;
}

std::vector<std::set<Tile *> >& GameState::getRacks() {
	return racks;
}

const std::vector<std::set<Tile *> >& GameState::getRacks() const {
	return racks;
}

Board& GameState::getBoard() {
	return board;
}

const Board& GameState::getBoard() const {
	return board;
}

std::vector<int>& GameState::getScores() {
	return scores;
}

const std::vector<int>& GameState::getScores() const {
	return scores;
}

Game *GameState::getGame() const {
	return game;
}

bool GameState::isFinal() const {
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

PlayerState::PlayerState(const GameState &s) : GameState(s) {}

const Board& PlayerState::getBoard() {
	return GameState::getBoard();
}

const std::set<Tile *>& PlayerState::getRack() {
	return GameState::getRacks().at(getTurn());
}

const std::vector<int>& PlayerState::getScores() {
	return GameState::getScores();
}

int PlayerState::getPlayerCount() {
	return GameState::getGame()->getPlayerCount();
}

void PlayerState::applyDecision(const PlayerDecision &decision) {
	GameState::applyDecision(decision);
}

