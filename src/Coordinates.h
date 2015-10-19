#ifndef COORDINATES_H_
#define COORDINATES_H_

#include <cstdint>

struct Coordinates {
	Coordinates(uint32_t row, uint32_t column)
			: row(row),
			  column(column) {
	}

	uint32_t row;
	uint32_t column;
};

#endif
