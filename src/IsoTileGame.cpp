/*
 * IsoTileGame.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include "IsoTileGame.h"

#include <algorithm>

#include "GameState.h"

IsoTileGame::IsoTileGame(std::vector<std::unique_ptr<Player>> players)
	: Game(std::move(players)) {}

IsoTileGame::IsoTileGame(IsoTileGame&& other)
		: Game(std::move(other)) {
}

IsoTileGame& IsoTileGame::operator=(IsoTileGame&& other) {
	std::swap(players_, other.players_);
	std::swap(currentState_, other.currentState_);
	std::swap(stateHistory_, other.stateHistory_);
	std::swap(decisionHistory_, other.decisionHistory_);
	return *this;
}

Tileset IsoTileGame::getInitialBag() {
	Tileset bag;
	std::vector<TileGroup> tileGroups = getTileGroups();
	for (const TileGroup& group : tileGroups) {
		for (int j = 0; j < group.multiplicity; j++) {
			std::shared_ptr<Tile> t;
			if (group.letter == L'_') {
				t.reset(new BlankTile());
			} else {
				t.reset(new Tile(group.letter, group.points, group.color));
			}
			bag.insert(t);
		}
	}
	return bag;
}

Tileset IsoTileGame::findTilesForPlayerRack(const GameState& state, const wchar_t *rackLetters) {
	int turn = state.getTurn();
	Rack rack = state.getRacks().at(turn); /* Create copy of the rack to remove found items from rack
	                                          in order to avoid reporting that a letter is found
	                                          multiple times using the same tile */
	Bag bag(state.getBag()); /* Create copy of the bag to safely remove found entries
	                          * in order to avoid adding the same tile twice. */
	Tileset tiles;

	const wchar_t* p = rackLetters;
	while (*p) {
		wchar_t letter = *p;
		bool found = false;
		for (const auto& tilePtr : rack) {
			if ((*p == L'_' && tilePtr->isBlank()) || tilePtr->getLetter() == letter) {
				auto newEnd = std::remove(rack.begin(), rack.end(), tilePtr);
				rack.erase(newEnd);
				found = true;
				break;
			}
		}
		if (!found) {
			for (const auto& tilePtr : bag) {
				if ((*p == L'_' && tilePtr->isBlank()) || tilePtr->getLetter() == letter) {
					tiles.insert(tilePtr);
					bag.erase(tilePtr);
					found = true;
					break;
				}
			}
		}
		if (!found) {
			throw "Failed to find tile for player rack.";
		}
		p++;
	}

	return tiles;
}

Tiles IsoTileGame::findTilesForPlayerMove(const GameState& state, int row, int column, Direction direction, const wchar_t* wordLetters) {
	int turn = state.getTurn();
	Rack rack = Rack(state.getRacks().at(turn));
	Tiles moveTiles;
	bool blankTile = false;

	const wchar_t *p = wordLetters;
	while (*p) {
		wchar_t letter = *p;

		if (*p == L'[') {
			blankTile = true;
		} else if (*p != L']') {
			bool found = false;
			if (state.getTiles()[row][column] == NULL) {
				for (const auto tilePtr : rack) {
					if (blankTile && tilePtr->isBlank()) {
						BlankTile *blank = static_cast<BlankTile *>(tilePtr.get());
						blank->fillLetter(letter);  //TODO do not store blank assignment in the tile!
						found = true;
					} else if (tilePtr->getLetter() == letter) {
						found = true;
					}

					if (found) {
						auto newEnd = std::remove(rack.begin(), rack.end(), tilePtr);
						rack.erase(newEnd);
						moveTiles.push_back(tilePtr);
						break;
					}
				}
				if (!found) {
					throw "Failed to find tile for player move.";
				}
			}

			if (direction == Direction::HORIZONTAL) {
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
