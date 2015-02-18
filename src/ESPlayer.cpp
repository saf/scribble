/*
 * ESPlayer.cpp
 *
 *  Created on: 16-02-2015
 *      Author: saf
 */

#include "ESPlayer.h"

#include <bitset>

ESPlayer::ESPlayer() {}

ESPlayer::~ESPlayer() {}

void ESPlayer::gameStarts(int yourId, const PlayerState &state) {}

void ESPlayer::playerDecisionMade(int playerId, const PlayerDecision &decision, const PlayerState &newState) {}

static void getBoardRowsAndColumns(const Board &b, std::vector<std::vector<wchar_t> >& rows, std::vector<std::vector<wchar_t> >& cols) {
	int width = b.getWidth();
	int height = b.getHeight();

	for (int row = 0; row < height; row++) {
		rows[row].resize(width);
	}
	for (int col = 0; col < width; col++) {
		cols[col].resize(height);
	}

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			const Tile *t = b.getTile(row, col);
			wchar_t ch = t != NULL ? t->getLetter() : L'.';
			rows[row][col] = ch;
			cols[col][row] = ch;
		}
	}
}

struct PlayerDecision ESPlayer::makeDecision(const PlayerState &state) {
	const Board& b = state.getBoard();
	std::vector<Hook> hooks = getHooks(b);
	std::set<Tile *> rack = state.getRack();
	std::vector<Tile *> tiles(rack.size());
	std::copy(rack.begin(), rack.end(), tiles.begin());

	std::vector<std::vector<wchar_t> > rows;
	std::vector<std::vector<wchar_t> > columns;
	getBoardRowsAndColumns(b, rows, columns);

	for (std::vector<Hook>::iterator it = hooks.begin(); it != hooks.end(); it++) {

	}

	PlayerDecision decision(PlayerDecision::PASS, PlayerDecision::Data());
	return decision;
}

std::vector<ESPlayer::Hook> ESPlayer::getHooks(const Board& board) {
	int width = board.getWidth();
	int height = board.getHeight();
	std::vector<ESPlayer::Hook> hooks;
	std::vector<std::vector<std::vector<int> > > hookAvailable;

	hookAvailable.resize(height);
	for (int row = 0; row < height; row++) {
		hookAvailable[row].resize(width);
		for (int col = 0; col < width; col++) {
			hookAvailable[row][col].resize(4);
			if (board.getTile(row, col) != NULL) {
				if (row > 0 && board.getTile(row - 1, col) == NULL) {
					Hook h = { row, col, UP };
					hookAvailable[row][col][UP] = true;
					hooks.push_back(h);
				}
				if (row < height - 1 && board.getTile(row + 1, col) == NULL) {
					Hook h = { row, col, DOWN };
					hookAvailable[row][col][DOWN] = true;
					hooks.push_back(h);
				}
				if (col > 0 && board.getTile(row, col - 1) == NULL) {
					Hook h = { row, col, LEFT };
					hookAvailable[row][col][LEFT] = true;
					hooks.push_back(h);
				}
				if (col < width - 1 && board.getTile(row, col + 1) == NULL) {
					Hook h = { row, col, RIGHT };
					hookAvailable[row][col][RIGHT] = true;
					hooks.push_back(h);
				}
			}
			else {
				if (((row + 1 < height && board.getTile(row + 1, col) != NULL) || (row - 1 >= 0 && board.getTile(row - 1, col) != NULL))
						&& !(col + 1 < width && board.getTile(row, col + 1) != NULL) && !(col > 0 && board.getTile(row, col - 1) != NULL)) {
					if (col > 0 && !hookAvailable[row][col - 1][LEFT] && !hookAvailable[row][col - 1][RIGHT]) {
						Hook h = { row, col, LEFT };
						hookAvailable[row][col][LEFT] = true;
						hooks.push_back(h);
					}
					if (col < width - 1 && board.getTile(row, col + 1) == NULL) {
						Hook h = { row, col, RIGHT };
						hookAvailable[row][col][RIGHT] = true;
						hooks.push_back(h);
					}
				}
				if (((col + 1 < width && board.getTile(row, col + 1) != NULL) || (col - 1 >= 0 && board.getTile(row, col - 1) != NULL))
						&& !(row + 1 < height && board.getTile(row + 1, col) != NULL) && !(row > 0 && board.getTile(row - 1, col) != NULL)) {
					if (row > 0 && !hookAvailable[row - 1][col][UP] && !hookAvailable[row - 1][col][DOWN] && !(row < height - 1 && board.getTile(row + 1, col) != NULL)) {
						Hook h = { row, col, UP };
						hookAvailable[row][col][UP] = true;
						hooks.push_back(h);
					}
					if (row < height - 1 && board.getTile(row + 1, col) == NULL) {
						Hook h = { row, col, DOWN };
						hookAvailable[row][col][DOWN] = true;
						hooks.push_back(h);
					}
				}
			}
		}
	}

	return hooks;
}
