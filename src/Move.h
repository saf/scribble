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
	Move(Coordinates start, Direction direction, Tiles tiles = Tiles(),
			BlankAssignments blankAssignment = BlankAssignments());

	Coordinates getCoordinates() const;
	uint getStartRow() const;
	uint getStartColumn() const;
	Direction getDirection() const;

	Tiles& getTiles();
	const Tiles& getTiles() const;

	BlankAssignments& getBlankAssignments();
	const BlankAssignments& getBlankAssignments() const;

private:
	Coordinates start_;
	Direction direction_;
	Tiles tiles_;
	BlankAssignments blankAssignments_;
};


#endif /* MOVE_H_ */
