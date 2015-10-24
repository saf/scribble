#ifndef COORDINATES_H_
#define COORDINATES_H_

#include <cstdint>

#include "Direction.h"

struct Coordinates {
	Coordinates(uint row, uint column)
			: row(row),
			  column(column) {
	}

	/// At which position on a line with the specific direction are we?
	uint offsetOnLine(Direction direction) {
		if (direction == Direction::HORIZONTAL) {
			return column;
		} else {
			return row;
		}
	}

	uint row;
	uint column;
};

#endif
