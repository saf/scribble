/*
 * Move.cpp
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */

#include "Move.h"

Move::Move(int startRow, int startColumn, enum Direction direction)
		: startRow(startRow), startColumn(startColumn), direction(direction) {}

Move::Move(int startRow, int startColumn, enum Direction direction, std::vector<Tile *>& tiles)
		: startRow(startRow), startColumn(startColumn), direction(direction), tiles(tiles) {}

Move::Move(int startRow, int startColumn, enum Direction direction, std::vector<Tile *>& tiles, std::vector<wchar_t>& blankAssignment)
		: startRow(startRow), startColumn(startColumn), direction(direction), tiles(tiles), blankAssignment(blankAssignment) {}

int Move::getStartRow() {
	return this->startRow;
}

int Move::getStartColumn() {
	return this->startColumn;
}

enum Move::Direction Move::getDirection() {
	return this->direction;
}

std::vector<Tile *>& Move::getTiles() {
	return tiles;
}

const std::vector<Tile *>& Move::getTiles() const {
	return tiles;
}

std::vector<wchar_t>& Move::getBlankAssignment() {
	return blankAssignment;
}

const std::vector<wchar_t>& Move::getBlankAssignment() const {
	return blankAssignment;
}
