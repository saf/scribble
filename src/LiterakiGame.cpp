/*
 * Literaki.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include <assert.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>

#include "Decision.h"
#include "Fields.h"
#include "GameState.h"
#include "IdlePlayer.h"
#include "LiterakiGame.h"
#include "LiterakiBoardPrinter.h"

LiterakiGame::LiterakiGame(std::vector<std::unique_ptr<Player>> players)
	: IsoTileGame(std::move(players)) {}

LiterakiGame::LiterakiGame(LiterakiGame&& other) : IsoTileGame(std::move(other)) {
}

LiterakiGame& LiterakiGame::operator=(LiterakiGame&& other) {
	std::swap(players, other.players);
	std::swap(currentState, other.currentState);
	std::swap(stateHistory, other.stateHistory);
	std::swap(decisionHistory, other.decisionHistory);
	return *this;
}

const int LiterakiGame::RACK_SIZE = 7;

std::vector<IsoTileGame::TileGroup> LiterakiGame::getTileGroups() {
	return std::vector<TileGroup>{
		{ 9, L'a', 1, LiterakiGame::YELLOW },
		{ 1, L'ą', 5, LiterakiGame::RED },
		{ 2, L'b', 3, LiterakiGame::BLUE },
		{ 3, L'c', 2, LiterakiGame::GREEN },
		{ 1, L'ć', 5, LiterakiGame::RED },
		{ 3, L'd', 2, LiterakiGame::GREEN },
		{ 7, L'e', 1, LiterakiGame::YELLOW },
		{ 1, L'ę', 5, LiterakiGame::RED },
		{ 1, L'f', 5, LiterakiGame::RED },
		{ 2, L'g', 3, LiterakiGame::BLUE },
		{ 2, L'h', 3, LiterakiGame::BLUE },
		{ 8, L'i', 1, LiterakiGame::YELLOW },
		{ 2, L'j', 3, LiterakiGame::BLUE },
		{ 3, L'k', 2, LiterakiGame::GREEN },
		{ 3, L'l', 2, LiterakiGame::GREEN },
		{ 2, L'ł', 3, LiterakiGame::BLUE },
		{ 3, L'm', 2, LiterakiGame::GREEN },
		{ 5, L'n', 1, LiterakiGame::YELLOW },
		{ 1, L'ń', 5, LiterakiGame::RED },
		{ 6, L'o', 1, LiterakiGame::YELLOW },
		{ 1, L'ó', 5, LiterakiGame::RED },
		{ 3, L'p', 2, LiterakiGame::GREEN },
		{ 4, L'r', 1, LiterakiGame::YELLOW },
		{ 4, L's', 1, LiterakiGame::YELLOW },
		{ 1, L'ś', 5, LiterakiGame::RED },
		{ 3, L't', 2, LiterakiGame::GREEN },
		{ 2, L'u', 3, LiterakiGame::BLUE },
		{ 4, L'w', 1, LiterakiGame::YELLOW },
		{ 4, L'y', 2, LiterakiGame::GREEN },
		{ 5, L'z', 1, LiterakiGame::YELLOW },
		{ 1, L'ź', 5, LiterakiGame::RED },
		{ 1, L'ż', 5, LiterakiGame::RED },
		{ 2, L'_' },
	};
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
		}
	}

	return b;
}

int LiterakiGame::getRackSize() const {
	return RACK_SIZE;
}

LiterakiGame LiterakiGame::readFromStream(std::wistream &stream) {
	Players players;
	players.emplace_back(new IdlePlayer());
	players.emplace_back(new IdlePlayer());
	LiterakiGame game(std::move(players));
	game.initializeState();

	int turn = 0;
	int round = 1;

	while (!stream.eof()) {
		wchar_t rack[RACK_SIZE + 2];
		wchar_t column;
		int row;
		wchar_t direction;
		wchar_t plus;
		std::wstring words;
		int points;
		std::wstring line;
		std::getline(stream, line);
		const wchar_t *str = line.c_str();

		if (strchr("#\r\n", str[0])) {
			/* Skip comments & blank lines */
			continue;
		}

		std::wistringstream lineStream(line);

		if (turn == 0) {
			int readRound;
			wchar_t dot;
			lineStream >> readRound >> dot;
			assert(readRound == round);
		}

		lineStream >> rack >> column >> row >> direction >> words >> plus >> points;

		if (lineStream.good()) {
			/* Word move.
			 *
			 * There is a colon in the 'rack' string, so we need to get rid of that. */
			rack[wcslen(rack) - 1] = L'\0';

			int col = column - 'a';

			size_t firstSlash = words.find_first_of(L'/');
			std::wstring mainWord = firstSlash != std::wstring::npos ? words.substr(0, firstSlash) : words;

			Tileset missingRackTiles = IsoTileGame::findTilesForPlayerRack(game.getCurrentState(), rack);
			assert(missingRackTiles.size() + game.getCurrentState().getRacks()[turn].size() == wcslen(rack));
			if (game.stateHistory.size() > 0) {
				game.stateHistory.back()->repopulateRack(turn, missingRackTiles);
			}
			game.currentState->repopulateRack(turn, missingRackTiles);

			Move::Direction dir = direction == L'+'
					? Move::Direction::VERTICAL
					: Move::Direction::HORIZONTAL;
			Tiles moveTiles = IsoTileGame::findTilesForPlayerMove(
					game.getCurrentState(), row - 1, col, dir, mainWord.c_str()
			);
			std::shared_ptr<MoveDecision> decision = std::shared_ptr<MoveDecision>(
					new MoveDecision(Move(Coordinates(row - 1, col), dir, moveTiles))
			);

			assert(points == game.getCurrentState().getBoard().getMoveScore(decision->getMove()));
			game.applyDecision(decision);
		} else {
			game.applyDecision(std::shared_ptr<Decision>(new PassDecision()));
		}

		if (turn == 1) {
			round++;
		}
		turn = (turn + 1) % 2;
	}

	return game;
}
