/*
 * Move.cpp
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */

#include "Move.h"

Move::Move(Coordinates start, Direction direction, Tiles tiles, BlankAssignments blankAssignments)
		: start_(std::move(start)),
		  direction_(direction),
		  tiles_(std::move(tiles)),
		  blankAssignments_(std::move(blankAssignments)) {
}

int Move::getStartRow() const {
	return this->start_.row;
}

int Move::getStartColumn() const {
	return this->start_.column;
}

enum Move::Direction Move::getDirection() const {
	return this->direction_;
}

Tiles& Move::getTiles() {
	return tiles_;
}

const Tiles& Move::getTiles() const {
	return tiles_;
}

BlankAssignments& Move::getBlankAssignments() {
	return blankAssignments_;
}

const BlankAssignments& Move::getBlankAssignments() const {
	return blankAssignments_;
}
