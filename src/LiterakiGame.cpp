/*
 * Literaki.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include <cmath>

#include "Literaki.h"
#include "Fields.h"

LiterakiGame::LiterakiGame(std::vector<Player *> players)
	: IsoTileGame(players) {}

LiterakiGame::~LiterakiGame() {
	for (std::vector<Field *>::iterator it = myFields.begin(); it != myFields.end(); it++) {
		delete *it;
	}
}

static const struct IsoTileGame::TileGroup Literaki_tileGroups[] = {
		{ 9, L'A', LiterakiGame::YELLOW },
		{ 1, L'Ą', LiterakiGame::RED },
		{ 2, L'B', LiterakiGame::BLUE },
		{ 3, L'C', LiterakiGame::GREEN },
		{ 1, L'Ć', LiterakiGame::RED },
		{ 3, L'D', LiterakiGame::GREEN },
		{ 7, L'E', LiterakiGame::YELLOW },
		{ 1, L'Ę', LiterakiGame::RED },
		{ 1, L'F', LiterakiGame::RED },
		{ 2, L'G', LiterakiGame::BLUE },
		{ 2, L'H', LiterakiGame::BLUE },
		{ 8, L'I', LiterakiGame::YELLOW },
		{ 2, L'J', LiterakiGame::BLUE },
		{ 3, L'K', LiterakiGame::GREEN },
		{ 3, L'L', LiterakiGame::GREEN },
		{ 2, L'Ł', LiterakiGame::BLUE },
		{ 3, L'M', LiterakiGame::GREEN },
		{ 5, L'N', LiterakiGame::YELLOW },
		{ 1, L'Ń', LiterakiGame::RED },
		{ 6, L'O', LiterakiGame::YELLOW },
		{ 1, L'Ó', LiterakiGame::RED },
		{ 3, L'P', LiterakiGame::GREEN },
		{ 4, L'R', LiterakiGame::YELLOW },
		{ 4, L'S', LiterakiGame::YELLOW },
		{ 1, L'Ś', LiterakiGame::RED },
		{ 3, L'T', LiterakiGame::GREEN },
		{ 2, L'U', LiterakiGame::BLUE },
		{ 4, L'W', LiterakiGame::YELLOW },
		{ 4, L'Y', LiterakiGame::GREEN },
		{ 5, L'Z', LiterakiGame::YELLOW },
		{ 1, L'Ź', LiterakiGame::RED },
		{ 1, L'Ż', LiterakiGame::RED },
};
const struct IsoTileGame::TileGroup *LiterakiGame::tileGroups = &Literaki_tileGroups[0];
const int LiterakiGame::tileGroupCount = sizeof(Literaki_tileGroups) / sizeof(IsoTileGame::TileGroup);

const int Literaki::RACK_SIZE = 7;

const IsoTileGame::TileGroup *LiterakiGame::getTileGroups() {
	return tileGroups;
}

int LiterakiGame::getTileGroupCount() {
	return tileGroupCount;
}

Board LiterakiGame::getInitialBoard() {
	Board b(15, 15);

	for (int r = 0; r < 15; r++) {
		for (int c = 0; c < 15; c++) {
			/* The special fields are organized in city-metric
			 * circles around the center, so we'll
			 * build the construction of the board upon that. */
			Field *f;
			int distanceFromCenter = std::abs(r - 7) + std::abs(c - 7);
			if (distanceFromCenter == 0) {
				f = new ColoredField(3, RED);
			} else if (distanceFromCenter == 2) {
				f = new ColoredField(3, BLUE);
			} else if (distanceFromCenter == 5) {
				f = new ColoredField(3, YELLOW);
			} else if (distanceFromCenter == 9) {
				f = new ColoredField(3, GREEN);
			} else if (distanceFromCenter == 14) {
				f = new ColoredField(3, RED);
			} else if (distanceFromCenter == 7) {
				if (std::abs(r - 7) <= 1 || std::abs(c - 7) <= 1) {
					f = new ColoredField(3, RED);
				} else {
					f = new MultiplicativeWordBonusField(2);
				}
			} else if (distanceFromCenter == 12 && r != 1 && r != 13) {
				f = new MultiplicativeWordBonusField(3);
			} else {
				f = new PlainField();
			}

			b.setField(r, c, f);
			myFields.push_back(f);
		}
	}

	return b;
}

int Literaki::getRackSize() {
	return RACK_SIZE;
}
