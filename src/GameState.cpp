/*
 * GameState.cpp
 *
 *  Created on: 15-02-2015
 *      Author: saf
 */

#include <algorithm>
#include "GameState.h"

GameState::GameState(Game& game, Board& board, std::set<Tile *>& bag)
		: game(game),
		  turn(0),
		  board(board),
		  bag(bag),
		  racks(game.getPlayerCount()),
		  scores(game.getPlayerCount(), 0) {
}

GameState::GameState(const GameState& other)
		: game(other.game),
		  turn(other.turn),
		  board(other.board),
		  bag(other.bag),
		  racks(other.racks),
		  scores(other.scores) {
}

void GameState::repopulateRack(int playerId) {
	std::set<Tile *>& rack = racks[playerId];
	int count = game.getRackSize() - rack.size();
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

std::shared_ptr<GameState> GameState::stateAfterDecision(const PlayerDecision &decision) const {
	std::shared_ptr<GameState> newState(new GameState(*this));

	switch (decision.type) {
		case PlayerDecision::MOVE: {
			Move *move = decision.data.move;
			int score = this->board.getMoveScore(*move);
			newState->scores[turn] += score;
			for (std::vector<Tile *>::const_iterator it = move->getTiles().begin(); it != move->getTiles().end(); it++) {
				newState->racks[turn].erase(*it);
			}
			newState->board.applyMove(*decision.data.move);
			break;
		}
		case PlayerDecision::EXCHANGE: {
			const std::vector<Tile *> &tiles = *decision.data.exchangedTiles;
			for (std::vector<Tile *>::const_iterator it = tiles.begin(); it != tiles.end(); it++) {
				newState->racks[turn].erase(*it);
				newState->hand.push_back(*it);
			}
			break;
		}
		case PlayerDecision::PASS: {
			break;
		}
	}

	return newState;
}

int GameState::getTurn() const {
	return turn;
}

void GameState::advanceTurn() {
	this->turn = (this->turn + 1) % game.getPlayerCount();
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

const Game& GameState::getGame() const {
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

PlayerState::PlayerState(const std::shared_ptr<GameState> state, int playerId)
		: state_(state),
		  playerId_(playerId) {
}

const Board& PlayerState::getBoard() const {
	return state_->getBoard();
}

const std::set<Tile *>& PlayerState::getRack() const {
	return state_->getRacks().at(playerId_);
}

const std::vector<int>& PlayerState::getScores() const {
	return state_->getScores();
}

int PlayerState::getPlayerCount() const {
	return state_->getGame().getPlayerCount();
}

PlayerState PlayerState::applyDecision(const PlayerDecision &decision) {
	auto newState = state_->stateAfterDecision(decision);
	return PlayerState(std::move(newState), playerId_);
}

