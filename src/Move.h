/*
 * Move.h
 *
 * Represents a single move - placing a word on the board.
 *
 *  Created on: 30-12-2014
 *      Author: saf
 */

#ifndef MOVE_H_
#define MOVE_H_

#include "BlankAssignment.h"
#include "Coordinates.h"
#include "Tile.h"

class Move {
public:
	enum class Direction { HORIZONTAL, VERTICAL };

	Move(Coordinates start, Direction direction, Tiles tiles = Tiles(),
			BlankAssignments blankAssignment = BlankAssignments());

	int getStartRow() const;
	int getStartColumn() const;
	enum Direction getDirection() const;

	Tiles& getTiles();
	const Tiles& getTiles() const;

	BlankAssignments& getBlankAssignments();
	const BlankAssignments& getBlankAssignments() const;

private:
	Coordinates start_;
	enum Direction direction_;
	Tiles tiles_;
	BlankAssignments blankAssignments_;
};


#endif /* MOVE_H_ */
