/*
 * Move.cpp
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */

#include "Move.h"

Move::Move(int startRow, int startColumn, enum Direction direction, std::vector<Tile *> *tiles) {
	this->startRow = startRow;
	this->startColumn = startColumn;
	this->direction = direction;
	this->tiles = tiles;
}

int Move::getStartRow() {
	return this->startRow;
}

int Move::getStartColumn() {
	return this->startColumn;
}

enum Move::Direction Move::getDirection() {
	return this->direction;
}

std::vector<Tile *> *Move::getTiles() {
	return this->tiles;
}
