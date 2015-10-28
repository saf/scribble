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


