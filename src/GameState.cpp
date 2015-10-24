/*
 * GameState.cpp
 *
 *  Created on: 15-02-2015
 *      Author: saf
 */

#include "GameState.h"

#include <algorithm>

#include "Decision.h"

GameState::GameState(Game& game, Board& board, Bag& bag)
		: game(game),
		  turn(0),
		  tiles(board),
		  bag(bag),
		  racks(game.getPlayerCount()),
		  scores(game.getPlayerCount(), 0) {
}

GameState::GameState(const GameState& other)
		: game(other.game),
		  turn(other.turn),
		  tiles(other.tiles),
		  bag(other.bag),
		  racks(other.racks),
		  scores(other.scores) {
}

void GameState::repopulateRack(int playerId) {
	Rack& rack = racks[playerId];
	int count = game.getRackSize() - rack.size();
	Tileset tiles;

	if (count > 0) {
		std::vector<std::shared_ptr<Tile>> shuffledBag;
		for (const std::shared_ptr<Tile>& tilePtr : bag) {
			shuffledBag.push_back(tilePtr);
		}
		random_shuffle(shuffledBag.begin(), shuffledBag.end());

		while (count) {
			auto& tilePtr = shuffledBag.back();
			tiles.insert(tilePtr);
			shuffledBag.pop_back();
		}
	}
	repopulateRack(playerId, tiles);
}

void GameState::repopulateRack(int playerId, const Tileset& tiles) {
	for (const auto& tilePtr : tiles) {
		bag.erase(tilePtr);
		racks[playerId].insert(tilePtr);
	}
}

std::shared_ptr<GameState> GameState::stateAfterDecision(const Decision &decision) const {
	std::shared_ptr<GameState> newState(new GameState(*this));
	decision.applyToState(*newState);
	return newState;
}

void GameState::applyMoveDecision(const MoveDecision& decision) {
	const Move& move = decision.getMove();
	int score = game.score(tiles, move);
	scores[turn] += score;

	Rack& rack = racks[turn];

	for (const std::shared_ptr<Tile>& tile : move.getTiles()) {
		auto it = rack.begin();
		while (it != rack.end()) {
			if (*it == tile) {
				it = rack.erase(it);
			} else {
				it++;
			}
		}
	}

	tiles.applyMove(move);
}

void GameState::applyExchangeDecision(const TileExchangeDecision& decision) {
	for (const auto& tile : decision.getExchangedTiles()) {
		racks[turn].erase(tile);
		hand.insert(tile);
	}
}

void GameState::applyPassDecision(const PassDecision& decision) {
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

Bag& GameState::getBag() {
	return bag;
}

const Bag& GameState::getBag() const {
	return bag;
}

const std::vector<Rack>& GameState::getRacks() const {
	return racks;
}

const std::vector<int>& GameState::getScores() const {
	return scores;
}

const TilePlacement& GameState::getTiles() const {
	return tiles;
}

const Game& GameState::getGame() const {
	return game;
}

bool GameState::isFinal() const {
	if (!bag.empty()) {
		return false;
	} else {
		return std::any_of(racks.begin(), racks.end(), [](const Rack& rack) {
			return rack.empty();
		});
	}
}

PlayerState::PlayerState(const std::shared_ptr<GameState> state, int playerId)
		: state_(state),
		  playerId_(playerId) {
}

const Board& PlayerState::getBoard() const {
	return state_->getGame().getBoard();
}

const TilePlacement& PlayerState::getTiles() const {
	return state_->getTiles();
}

const Rack& PlayerState::getRack() const {
	return state_->getRacks().at(playerId_);
}

const std::vector<int>& PlayerState::getScores() const {
	return state_->getScores();
}

int PlayerState::getPlayerCount() const {
	return state_->getGame().getPlayerCount();
}

PlayerState PlayerState::applyDecision(const Decision &decision) {
	auto newState = state_->stateAfterDecision(decision);
	return PlayerState(std::move(newState), playerId_);
}

