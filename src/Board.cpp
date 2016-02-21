/*
 * Board.cpp
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#include "Board.h"

Board::Board(int height, int width, std::function<std::unique_ptr<Field>(uint, uint)> fieldInitializer)
		: fields(height, width, fieldInitializer) {
}

Board::Board(Board&& other)
		: fields(std::move(other.fields)) {
}

Board::~Board() {}

Board& Board::operator=(Board&& other) {
	fields = std::move(other.fields);
	return *this;
}

int Board::getWidth() const {
	return fields.getColumnCount();
}

int Board::getHeight() const {
	return fields.getRowCount();
}

const Field& Board::getField(const Coordinates& coords) const {
	return getField(coords.row, coords.column);
}

const Field& Board::getField(uint row, uint col) const {
	return *fields[row][col];
}
