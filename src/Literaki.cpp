/*
 * Literaki.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include <cmath>

#include "Literaki.h"
#include "Fields.h"

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
			/* The special fields are organized in city-metric
			 * circles around the center, so we'll
			 * build the construction of the board upon that. */

			int distanceFromCenter = std::abs(r - 7) + std::abs(c - 7);
			if (distanceFromCenter == 0) {
				b.setField(r, c, new ColoredField<RED, 3>());
			} else if (distanceFromCenter == 2) {
				b.setField(r, c, new ColoredField<BLUE, 3>());
			} else if (distanceFromCenter == 5) {
				b.setField(r, c, new ColoredField<YELLOW, 3>());
			} else if (distanceFromCenter == 9) {
				b.setField(r, c, new ColoredField<GREEN, 3>());
			} else if (distanceFromCenter == 14) {
				b.setField(r, c, new ColoredField<RED, 3>());
			} else if (distanceFromCenter == 7) {
				if (abs(r - 7) <= 1 || abs(c - 7) <= 1) {
					b.setField(r, c, new ColoredField<RED, 3>());
				} else {
					b.setField(r, c, new MultiplicativeWordBonusField<2>());
				}
			} else if (distanceFromCenter == 12 && r != 1 && r != 13) {
				b.setField(r, c, new MultiplicativeWordBonusField<3>());
			} else {
				b.setField(r, c, new PlainField());
			}
		}
	}

	return b;
}
