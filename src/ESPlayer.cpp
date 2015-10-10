/*
 * ESPlayer.cpp
 *
 *  Created on: 16-02-2015
 *      Author: saf
 */

#include "ESPlayer.h"

#include <algorithm>
#include <map>

#include "Decision.h"
#include "GameState.h"

ESPlayer::ESPlayer(ESDictionary& dict) : dictionary(dict) {}

ESPlayer::~ESPlayer() {}

void ESPlayer::gameStarts(int yourId, const PlayerState& state) {}

void ESPlayer::playerDecisionMade(int playerId, const Decision& decision, const PlayerState& newState) {}

static const ESBoardInfo getBoardInfo(const Board& b, std::vector<std::vector<wchar_t> >& rows, std::vector<std::vector<wchar_t> >& cols) {
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

	ESBoardInfo bi(b, rows, cols);
	return bi;
}

static void moveFoundCallback(Move *move, void *context) {
	std::vector<Move *> *moves = static_cast<std::vector<Move *> *>(context);
	moves->push_back(move);
}

std::shared_ptr<Decision> ESPlayer::makeDecision(const PlayerState& state) {
	const Board& b = state.getBoard();
	std::vector<ESHook> hooks = getHooks(b);
	const Rack& rack = state.getRack();
	std::vector<Tile *> tiles(rack.size());
	// TODO change this once Moves get shared pointers to tiles as well.
	std::transform(rack.begin(), rack.end(), std::back_inserter(tiles), [](const std::shared_ptr<Tile>& tile) {
		return tile.get();
	});

	std::vector<std::vector<wchar_t> > rows;
	std::vector<std::vector<wchar_t> > columns;
	ESBoardInfo bi = getBoardInfo(b, rows, columns);
	std::vector<Move *> moves;
	std::multimap<int, Move *> moveScores;

	for (std::vector<ESHook>::iterator it = hooks.begin(); it != hooks.end(); it++) {
		ESHook h = *it;
		dictionary.findMovesAtHook(bi, h, tiles, moveFoundCallback, &moves);
	}

	for (std::vector<Move *>::iterator it = moves.begin(); it != moves.end(); it++) {
		Move *m = *it;
		int score = state.getBoard().getMoveScore(*m);
		moveScores.insert(std::pair<int, Move *>(score, m));
	}

	std::multimap<int, Move *>::reverse_iterator rit = moveScores.rbegin();
	if (rit != moveScores.rend()) {
		return std::shared_ptr<Decision>(new MoveDecision(*(rit->second)));
	} else {
		return std::shared_ptr<Decision>(new PassDecision());
	}
}

std::vector<ESHook> ESPlayer::getHooks(const Board& board) {
	int width = board.getWidth();
	int height = board.getHeight();
	std::vector<ESHook> hooks;
	std::vector<std::vector<std::vector<int> > > hookAvailable;

	hookAvailable.resize(height);
	for (int row = 0; row < height; row++) {
		hookAvailable[row].resize(width);
		for (int col = 0; col < width; col++) {
			hookAvailable[row][col].resize(4);
			if (board.getTile(row, col) != NULL) {
				if (row == 0 || board.getTile(row - 1, col) == NULL) {
					ESHook hDown = { row, col, DOWN };
					hookAvailable[row][col][DOWN] = true;
					hooks.push_back(hDown);
					if (row > 0) {
						ESHook hUp = { row, col, UP };
						hookAvailable[row][col][UP] = true;
						hooks.push_back(hUp);
					}
				}
				if (col == 0 || board.getTile(row, col - 1) == NULL) {
					ESHook hRight = { row, col, RIGHT };
					hookAvailable[row][col][RIGHT] = true;
					hooks.push_back(hRight);
					if (col > 0) {
						ESHook hLeft = { row, col, LEFT };
						hookAvailable[row][col][LEFT] = true;
						hooks.push_back(hLeft);
					}
				}
			}
			else {
				if (((row + 1 < height && board.getTile(row + 1, col) != NULL) || (row - 1 >= 0 && board.getTile(row - 1, col) != NULL))
						&& !(col + 1 < width && board.getTile(row, col + 1) != NULL) && !(col > 0 && board.getTile(row, col - 1) != NULL)) {
					if (col > 0 && !hookAvailable[row][col - 1][LEFT] && !hookAvailable[row][col - 1][RIGHT]) {
						ESHook h = { row, col, LEFT };
						hookAvailable[row][col][LEFT] = true;
						hooks.push_back(h);
					}
					if (col < width - 1 && board.getTile(row, col + 1) == NULL) {
						ESHook h = { row, col, RIGHT };
						hookAvailable[row][col][RIGHT] = true;
						hooks.push_back(h);
					}
				}
				if (((col + 1 < width && board.getTile(row, col + 1) != NULL) || (col - 1 >= 0 && board.getTile(row, col - 1) != NULL))
						&& !(row + 1 < height && board.getTile(row + 1, col) != NULL) && !(row > 0 && board.getTile(row - 1, col) != NULL)) {
					if (row > 0 && !hookAvailable[row - 1][col][UP] && !hookAvailable[row - 1][col][DOWN] && !(row < height - 1 && board.getTile(row + 1, col) != NULL)) {
						ESHook h = { row, col, UP };
						hookAvailable[row][col][UP] = true;
						hooks.push_back(h);
					}
					if (row < height - 1 && board.getTile(row + 1, col) == NULL) {
						ESHook h = { row, col, DOWN };
						hookAvailable[row][col][DOWN] = true;
						hooks.push_back(h);
					}
				}
			}
		}
	}

	return hooks;
}
