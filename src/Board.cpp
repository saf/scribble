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

Board::~Board() {}

Board::Board(const Board &board) {
	Board(board.height, board.width);
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
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

	if (row < 0 || col < 0 || row >= this->height || col >= this->width) {
		return false;
	}

	while (row < this->height && col < this->width && this->tiles[row][col] != NULL) {
		if (direction == Move::HORIZONTAL) {
			col++;
		} else {
			row++;
		}
	}

	for (std::vector<Tile *>::const_iterator it = tiles->begin(); it != tiles->end(); it++) {
		if (row >= this->height || col >= this->width) {
			return false;
		}

		if (direction == Move::HORIZONTAL) {
			col++;
		} else {
			row++;
		}

		while (row < this->height && col < this->width && this->tiles[row][col] != NULL) {
			if (direction == Move::HORIZONTAL) {
				col++;
			} else {
				row++;
			}
		}
	}

	return true;
}

int Board::getNewWordScore(std::vector<Tile *> *wordTiles, int startRow, int startColumn, enum Move::Direction direction, int tileIndex) {
	int score = 0;
	std::vector<Field *> newTileFields;
	std::vector<Field *> oldTileFields;
	int row = startRow - (direction == Move::VERTICAL ? 1 : 0);
	int col = startColumn - (direction == Move::HORIZONTAL ? 1 : 0);
	bool expansionFound = false;

	/* Account for existing tiles 'before' our tile in the new word */
	while (col >= 0 && row >= 0 && this->tiles[row][col] != NULL) {
		Field *field = this->fields[row][col];
		field->applyScore(*this->tiles[row][col], false, score);
		oldTileFields.push_back(field);

		if (direction == Move::HORIZONTAL) {
			col--;
		} else {
			row--;
		}
		expansionFound = true;
	}

	row = startRow;
	col = startColumn;
	if (tileIndex == -1) {
		for (std::vector<Tile *>::const_iterator it = wordTiles->begin(); it != wordTiles->end(); it++) {
			Field *field = this->fields[row][col];
			field->applyScore(**it, true, score);
			newTileFields.push_back(field);

			if (direction == Move::HORIZONTAL) {
				col++;
				while (col < this->width && this->tiles[row][col] != NULL) {
					field = this->fields[row][col];
					field->applyScore(*this->tiles[row][col], false, score);
					oldTileFields.push_back(field);
					col++;
				}
			} else {
				row++;
				while (row < this->height && this->tiles[row][col] != NULL) {
					field = this->fields[row][col];
					field->applyScore(*this->tiles[row][col], false, score);
					oldTileFields.push_back(field);
					row++;
				}
			}
		}
	} else {
		/* Skip our new letter for now. If we find an expansion into a word to which this letter
		 * contributes, it will be accounted for at the end of this function. */
		if (direction == Move::HORIZONTAL) {
			col++;
		} else {
			row++;
		}
	}

	if (direction == Move::HORIZONTAL) {
		while (col < this->width && this->tiles[row][col] != NULL) {
			Field *field = this->fields[row][col];
			field->applyScore(*this->tiles[row][col], false, score);
			oldTileFields.push_back(field);
			expansionFound = true;
			col++;
		}
	} else {
		while (row < this->height && this->tiles[row][col] != NULL) {
			Field *field = this->fields[row][col];
			field->applyScore(*this->tiles[row][col], false, score);
			oldTileFields.push_back(field);
			expansionFound = true;
			row++;
		}
	}

	if (tileIndex != -1 && expansionFound) {
		Field *field = this->fields[startRow][startColumn];
		field->applyScore(*wordTiles->at(tileIndex), true, score);
		newTileFields.push_back(field);
	}

	/* This uses a property which should hold for fields that word
	 * bonuses have the same results regardless of their order. */
	for (std::vector<Field *>::const_iterator it = newTileFields.begin(); it != newTileFields.end(); it++) {
		(*it)->changeWordScore(true, score);
	}
	for (std::vector<Field *>::const_iterator it = oldTileFields.begin(); it != oldTileFields.end(); it++) {
		(*it)->changeWordScore(false, score);
	}


	return score;
}

int Board::getMoveScore(Move &move) {
	enum Move::Direction direction = move.getDirection();
	enum Move::Direction orthogonal = direction == Move::HORIZONTAL ? Move::VERTICAL : Move::HORIZONTAL;
	int startRow = move.getStartRow();
	int startColumn = move.getStartColumn();
	std::vector<Tile *> *tiles = move.getTiles();
	int score = 0;
	int row = startRow;
	int col = startColumn;

	score += getNewWordScore(tiles, row, col, direction);

	for (int i = 0; i < tiles->size(); i++) {
		score += getNewWordScore(tiles, row, col, orthogonal, i);

		if (direction == Move::HORIZONTAL) {
			col++;
			while (col < this->width && this->tiles[row][col] != NULL) {
				col++;
			}
		} else {
			row++;
			while (row < this->height && this->tiles[row][col] != NULL) {
				row++;
			}
		}
	}

	return score;
}

void Board::applyMove(Move &move) {
	enum Move::Direction direction = move.getDirection();
	int row = move.getStartRow();
	int col = move.getStartColumn();
	std::vector<Tile *> *tiles = move.getTiles();

	if (!this->checkMove(move)) {
		throw InvalidMove();
	}

	for (std::vector<Tile *>::const_iterator it = tiles->begin(); it != tiles->end(); it++) {
		this->putTile(row, col, **it);

		if (direction == Move::HORIZONTAL) {
			col++;
			while (col < this->width && this->tiles[row][col] != NULL) {
				col++;
			}
		} else {
			row++;
			while (row < this->height && this->tiles[row][col] != NULL) {
				row++;
			}
		}
	}
}

void Board::setField(int row, int col, Field *f) {
	this->fields[row][col] = f;
}

const Field *Board::getField(int row, int col) {
	return this->fields[row][col];
}

void Board::setTile(int row, int col, Tile *t) {
	this->tiles[row][col] = t;
}

const Tile *Board::getTile(int row, int col) {
	return this->tiles[row][col];
}
