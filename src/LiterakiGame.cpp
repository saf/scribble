/*
 * Literaki.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include <cmath>
#include <iostream>
#include <cstring>
#include <assert.h>

#include "LiterakiGame.h"
#include "IdlePlayer.h"
#include "Fields.h"
#include "LiterakiBoardPrinter.h"

LiterakiGame::LiterakiGame(std::vector<Player *> players)
	: IsoTileGame(players) {}

LiterakiGame::~LiterakiGame() {
	for (std::vector<Field *>::iterator it = myFields.begin(); it != myFields.end(); it++) {
		delete *it;
	}
}

static const struct IsoTileGame::TileGroup Literaki_tileGroups[] = {
		{ 9, L'a', LiterakiGame::YELLOW },
		{ 1, L'ą', LiterakiGame::RED },
		{ 2, L'b', LiterakiGame::BLUE },
		{ 3, L'c', LiterakiGame::GREEN },
		{ 1, L'ć', LiterakiGame::RED },
		{ 3, L'd', LiterakiGame::GREEN },
		{ 7, L'e', LiterakiGame::YELLOW },
		{ 1, L'ę', LiterakiGame::RED },
		{ 1, L'f', LiterakiGame::RED },
		{ 2, L'g', LiterakiGame::BLUE },
		{ 2, L'h', LiterakiGame::BLUE },
		{ 8, L'i', LiterakiGame::YELLOW },
		{ 2, L'j', LiterakiGame::BLUE },
		{ 3, L'k', LiterakiGame::GREEN },
		{ 3, L'l', LiterakiGame::GREEN },
		{ 2, L'ł', LiterakiGame::BLUE },
		{ 3, L'm', LiterakiGame::GREEN },
		{ 5, L'n', LiterakiGame::YELLOW },
		{ 1, L'ń', LiterakiGame::RED },
		{ 6, L'o', LiterakiGame::YELLOW },
		{ 1, L'ó', LiterakiGame::RED },
		{ 3, L'p', LiterakiGame::GREEN },
		{ 4, L'r', LiterakiGame::YELLOW },
		{ 4, L's', LiterakiGame::YELLOW },
		{ 1, L'ś', LiterakiGame::RED },
		{ 3, L't', LiterakiGame::GREEN },
		{ 2, L'u', LiterakiGame::BLUE },
		{ 4, L'w', LiterakiGame::YELLOW },
		{ 4, L'y', LiterakiGame::GREEN },
		{ 5, L'z', LiterakiGame::YELLOW },
		{ 1, L'ź', LiterakiGame::RED },
		{ 1, L'ż', LiterakiGame::RED },
		{ 2, L'_' },
};
const struct IsoTileGame::TileGroup *LiterakiGame::tileGroups = &Literaki_tileGroups[0];
const int LiterakiGame::tileGroupCount = sizeof(Literaki_tileGroups) / sizeof(IsoTileGame::TileGroup);

const int LiterakiGame::RACK_SIZE = 7;

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

int LiterakiGame::getRackSize() {
	return RACK_SIZE;
}

LiterakiGame* LiterakiGame::readFromStream(std::wistream &s) {
	IdlePlayer player1;
	IdlePlayer player2;
	std::vector<Player *> players;
	players.push_back(&player1);
	players.push_back(&player2);
	LiterakiGame* game = new LiterakiGame(players);
	game->initializeState();

	LiterakiBoardPrinter prn;
	prn.printBoard(game->getCurrentState()->getBoard());

	int turn = 0;
	int round = 1;

	while (!s.eof()) {
		int readRound;
		wchar_t rack[RACK_SIZE + 1];
		char column[2];
		int row;
		char direction[2];
		wchar_t words[101];
		int points;
		std::wstring line;
		std::getline(s, line);
		const wchar_t *str = line.c_str();

		if (strchr("#\r\n", str[0])) {
			/* Skip comments & blank lines */
			continue;
		}

		if (turn == 0) {
			int chars;
			swscanf(line.c_str(), L"%d. %n", &readRound, &chars);
			assert(readRound == round);
			str += chars;
		}

		if (swscanf(str, L"%7ls: %1[a-o]%d%1[+-] %100ls +%d", rack, column, &row, direction, words, &points) == 6) {
			/* Word move */
			Decision::Data decisionData;
			int col = column[0] - 'a';
			wchar_t *mainWordEnd = wcschr(words, L'/');
			if (mainWordEnd != NULL) {
				*mainWordEnd = L'\0';
			}
			std::set<Tile *> playerRack = game->currentState->findTilesForPlayerRack(turn, rack);
			if (game->stateHistory.size() > 0) {
				game->stateHistory.back()->setRack(turn, playerRack);
			}
			game->currentState->setRack(turn, playerRack);

			Move::Direction dir = direction[0] == '+' ? Move::VERTICAL : Move::HORIZONTAL;
			std::vector<Tile *> moveTiles = game->currentState->findTilesForPlayerMove(turn, row - 1, col, dir, words);
			decisionData.move = new Move(row - 1, col, dir, &moveTiles);
			Decision decision(Decision::MOVE, decisionData);
			game->applyDecision(decision);
			prn.printBoard(game->getCurrentState()->getBoard());
		} else {
			Decision decision(Decision::PASS, Decision::Data());
			game->applyDecision(decision);
		}

		if (turn == 1) {
			round++;
		}
		turn = (turn + 1) % 2;
	}

	return game;
}
