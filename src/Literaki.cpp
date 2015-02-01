/*
 * Literaki.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include "Literaki.h"

Literaki::Literaki(std::vector<Player *> players)
	: IsoTileGame(players) {}

Literaki::~Literaki() {}

static const struct IsoTileGame::TileGroup Literaki_tileGroups[] = {
		{ 9, L'A', Literaki::YELLOW },
		{ 1, L'Ą', Literaki::RED },
		{ 2, L'B', Literaki::BLUE },
		{ 3, L'C', Literaki::GREEN },
		{ 1, L'Ć', Literaki::RED },
		{ 3, L'D', Literaki::GREEN },
		{ 7, L'E', Literaki::YELLOW },
		{ 1, L'Ę', Literaki::RED },
		{ 1, L'F', Literaki::RED },
		{ 2, L'G', Literaki::BLUE },
		{ 2, L'H', Literaki::BLUE },
		{ 8, L'I', Literaki::YELLOW },
		{ 2, L'J', Literaki::BLUE },
		{ 3, L'K', Literaki::GREEN },
		{ 3, L'L', Literaki::GREEN },
		{ 2, L'Ł', Literaki::BLUE },
		{ 3, L'M', Literaki::GREEN },
		{ 5, L'N', Literaki::YELLOW },
		{ 1, L'Ń', Literaki::RED },
		{ 6, L'O', Literaki::YELLOW },
		{ 1, L'Ó', Literaki::RED },
		{ 3, L'P', Literaki::GREEN },
		{ 4, L'R', Literaki::YELLOW },
		{ 4, L'S', Literaki::YELLOW },
		{ 1, L'Ś', Literaki::RED },
		{ 3, L'T', Literaki::GREEN },
		{ 2, L'U', Literaki::BLUE },
		{ 4, L'W', Literaki::YELLOW },
		{ 4, L'Y', Literaki::GREEN },
		{ 5, L'Z', Literaki::YELLOW },
		{ 1, L'Ź', Literaki::RED },
		{ 1, L'Ż', Literaki::RED },
};
const struct IsoTileGame::TileGroup *Literaki::tileGroups = &Literaki_tileGroups[0];
const int Literaki::tileGroupCount = sizeof(Literaki_tileGroups) / sizeof(IsoTileGame::TileGroup);

const IsoTileGame::TileGroup *Literaki::getTileGroups() {
	return tileGroups;
}

int Literaki::getTileGroupCount() {
	return tileGroupCount;
}

Board Literaki::getInitialBoard() {
	Board b(15, 15);

	for (int r = 0; r < 15; r++) {
		for (int c = 0; c < 15; c++) {
//			int distanceFromCenter = abs(r - 7) + abs(c - 7);
			/* TODO */
		}
	}

	return b;
}
