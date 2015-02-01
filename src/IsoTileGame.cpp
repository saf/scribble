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
			Tile *t = new Tile(group->letter, group->points, group->color);
			bag.insert(t);
			myTiles.push_back(t);
		}
		group++;
	}
	return bag;
}

