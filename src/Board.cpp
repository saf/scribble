/*
 * Board.cpp
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#include "Board.h"

Board::Board(int height, int width, std::function<std::unique_ptr<Field>(uint, uint)> fieldInitializer)
		: height(height),
		  width(width),
		  fields(height, width, fieldInitializer) {
}

Board::Board(Board&& other)
		: height(other.height),
		  width(other.width),
		  fields(std::move(other.fields)) {
}

Board::~Board() {}

Board& Board::operator=(Board&& other) {
	height = other.height;
	width = other.width;
	fields = std::move(other.fields);
	return *this;
}

int Board::getWidth() const {
	return this->width;
}

int Board::getHeight() const {
	return this->height;
}

const Field& Board::getField(const Coordinates& coords) const {
	return getField(coords.row, coords.column);
}

const Field& Board::getField(uint row, uint col) const {
	return *fields[row][col];
}
