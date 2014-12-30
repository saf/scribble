/*
 * Board.cpp
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#include "Board.h"

Board::Board(int height, int width) {
	this->height = height;
	this->width = width;

	this->rows.resize(height);
	this->columns.resize(width);
	this->fields.resize(height);
	this->tiles.resize(height);

	for (int i = 0; i < height; i++) {
		this->fields[i].resize(width);
		this->tiles[i].resize(width);
		this->rows[i].resize(width);
	}

	for (int i = 0; i < width; i++) {
		this->columns[i].resize(height);
	}
}

Board::~Board() {
	for (int i = 0; i < this->height; i++) {
		delete this->fields[i];
		delete this->tiles[i];
		delete this->rows[i];
	}

	for (int i = 0; i < this->width; i++) {
		delete [] this->columns[i];
	}

	delete this->fields;
	delete this->tiles;
	delete this->rows;
	delete this->columns;
}

Board::Board(const Board &board) {
	Board(board.height, board.width);
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->columns; j++) {
			this->fields[i][j] = board.fields[i][j];
			this->tiles[i][j] = board.tiles[i][j];
			this->rows[i][j] = board.rows[i][j];
			this->columns[j][i] = board.columns[j][i];
		}
	}
}

void Board::putTile(int row, int column, Tile &tile) {
	wchar_t letter = tile.getLetter();
	this->rows[row][column] = letter;
	this->columns[column][row] = letter;
	this->tiles[row][column] = &tile;
}

bool Board::checkMove(Move &move) {
	enum Move::Direction direction = move.getDirection();
	int startRow = move.getStartRow();
	int startColumn = move.getStartColumn();
	std::vector<Tile *> *tiles = move.getTiles();
	int row = startRow;
	int col = startColumn;

	if (startRow < 0 || startRow >= this->height || startColumn < 0 || startColumn >= this->width) {
		return false;
	}

	for (int i = 0; i < tiles->size(); i++) {
		while (this->tiles[row][col] != NULL && row < this->height && col < this->width) {
			if (direction == Move::Direction::HORIZONTAL) {
				col++;
			} else {
				row++;
			}
		}
		/* We did not get to end of move's tiles, but we are at board boundary */
		if (tiles[i + 1] != NULL && (row >= this->height || col >= this->width)) {
			return false;
		}
	}

	return true;
}

int Board::getNewWordScore(std::vector<Tile *> *tiles, int startRow, int startColumn, enum Move::Direction direction, int tileIndex = -1) {
	int score = 0;
	std::vector<wordScoreFunc> wordScoreFuncs;
	wordScoreFuncs.reserve(5);
	int row = startRow - (direction == Move::Direction::VERTICAL ? 1 : 0);
	int col = startColumn - (direction == Move::Direction::HORIZONTAL ? 1 : 0);

	/* Account for existing tiles 'before' our tile in the new word */
	while (col >= 0 && row >= 0 && this->tiles[row][col] != NULL) {
		this->fields[row][col]->applyScore(*this->tiles[row][col], false, score, wordScoreFuncs);
		if (direction == Move::Direction::HORIZONTAL) {
			col--;
		} else {
			row--;
		}
	}

	row = startRow;
	col = startColumn;
	if (tileIndex == -1) {
		for (std::vector<Tile *>::const_iterator it = tiles->begin(); it != tiles->end(); it++) {
			this->fields[row][col]->applyScore(**it, true, score, wordScoreFuncs);

			if (direction == Move::Direction::HORIZONTAL) {
				col++;
				while (col < this->width && this->tiles[row][col] != NULL) {
					this->fields[row][col]->applyScore(*tiles[row][col], false, score, wordScoreFuncs);
					col++;
				}
			} else {
				row++;
				while (row < this->height && this->tiles[row][col] != NULL) {
					this->fields[row][col]->applyScore(*tiles[row][col], false, score, wordScoreFuncs);
				}
			}
		}
	} else {
		this->fields[row][col]->applyScore(*tiles->at(tileIndex), true, score, wordScoreFuncs);
	}

	if (direction == Move::Direction::HORIZONTAL) {
		col++;
		while (col < this->width && this->tiles[row][col] != NULL) {
			this->fields[row][col]->applyScore(*tiles[row][col], false, score, wordScoreFuncs);
			col++;
		}
	} else {
		row++;
		while (row < this->height && this->tiles[row][col] != NULL) {
			this->fields[row][col]->applyScore(*tiles[row][col], false, score, wordScoreFuncs);
		}
	}

	for (std::vector<wordScoreFunc>::const_iterator it = wordScoreFuncs.begin(); it != wordScoreFuncs.end(); it++) {
		(*it)(score);
	}

	return score;
}

int Board::getMoveScore(Move &move) {
	enum Move::Direction direction = move.getDirection();
	enum Move::Direction orthogonal = direction == Move::Direction::HORIZONTAL ? Move::Direction::VERTICAL : Move::Direction::HORIZONTAL;
	int startRow = move.getStartRow();
	int startColumn = move.getStartColumn();
	std::vector<Tile *> *tiles = move.getTiles();
	int score = 0;
	int row = startRow;
	int col = startColumn;

	score += getNewWordScore(tiles, row, col, direction);

	for (int i = 0; i < tiles->size(); i++) {
		score += getNewWordScore(tiles, row, col, orthogonal, i);

		if (direction == Move::Direction::HORIZONTAL) {
			col++;
		} else {
			row++;
		}
	}

	return score;
}

void Board::applyMove(Move &move) {
	enum Move::Direction direction = move.getDirection();
	int row = move.getStartRow();
	int col = move.getStartColumn();
	std::vector<Tile *> *tiles = move.getTiles();

	for (std::vector<Tile *>::const_iterator it = tiles->begin(); it != tiles->end(); it++) {
		this->putTile(row, col, **it);

		if (direction == Move::Direction::HORIZONTAL) {
			col++;
			while (this->tiles[row][col] != NULL) {
				col++;
			}
		} else {
			row++;
			while (this->tiles[row][col] != NULL) {
				row++;
			}
		}
	}
}

