/*
 * IsoTileGame.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include "IsoTileGame.h"

IsoTileGame::IsoTileGame(std::vector<Player *> &players)
	: Game(players) {}

IsoTileGame::~IsoTileGame() {
	for (std::vector<Tile *>::iterator it = myTiles.begin(); it != myTiles.end(); it++) {
		delete *it;
	}
}

std::set<Tile *> IsoTileGame::getInitialBag() {
	std::set<Tile *> bag;
	const TileGroup *group = this->getTileGroups();
	for (int i = 0; i < this->getTileGroupCount(); i++) {
		for (int j = 0; j < group->multiplicity; j++) {
			Tile *t;
			if (group->letter == L'_') {
				t = new BlankTile();
			} else {
				t = new Tile(group->letter, group->points, group->color);
			}
			bag.insert(t);
			myTiles.push_back(t);
		}
		group++;
	}
	return bag;
}

std::vector<Tile *> IsoTileGame::findTilesForPlayerRack(const GameState &state, const wchar_t *rackLetters) {
	const wchar_t *p = rackLetters;
	int turn = state.getTurn();
	std::set<Tile *> rack = state.getRacks().at(turn);
	std::set<Tile *> bag = std::set<Tile *>(state.getBag()); /* Create copy of the bag to safely remove found entries. */
	std::vector<Tile *> tiles;

	while (*p) {
		wchar_t letter = *p;
		bool found = false;
		for (std::set<Tile *>::iterator it = rack.begin(); it != rack.end(); it++) {
			if ((*p == L'_' && (*it)->isBlank()) || (*it)->getLetter() == letter) {
				found = true;
				break;
			}
		}
		if (!found) {
			for (std::set<Tile *>::iterator it = bag.begin(); it != bag.end(); it++) {
				if ((*p == L'_' && (*it)->isBlank()) || (*it)->getLetter() == letter) {
					tiles.push_back(*it);
					bag.erase(*it);
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

std::vector<Tile *> IsoTileGame::findTilesForPlayerMove(const GameState &state, int row, int column, Move::Direction direction, const wchar_t *wordLetters) {
	int turn = state.getTurn();
	std::set<Tile *> rack = std::set<Tile *>(state.getRacks().at(turn));
	std::vector<Tile *> moveTiles;
	const wchar_t *p = wordLetters;
	bool blankTile;

	while (*p) {
		wchar_t letter = *p;

		if (*p == L'[') {
			blankTile = true;
		} else if (*p != L']') {
			bool found = false;
			if (state.getBoard().getTile(row, column) == NULL) {
				for (std::set<Tile *>::iterator it = rack.begin(); it != rack.end(); it++) {
					if (blankTile && (*it)->isBlank()) {
						BlankTile *blank = static_cast<BlankTile *>(*it);
						blank->fillLetter(letter);
						moveTiles.push_back(blank);
						rack.erase(blank);
						found = true;
						break;
					} else if ((*it)->getLetter() == letter) {
						moveTiles.push_back(*it);
						rack.erase(*it);
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
