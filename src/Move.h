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

#include <string>
#include <vector>

#include "Tile.h"

class Move {

public:
	enum Direction { HORIZONTAL, VERTICAL };

private:
	int startRow;
	int startColumn;
	enum Direction direction;
	std::vector<Tile *> tiles;
	std::vector<wchar_t> blankAssignment;

public:
	Move(int startRow, int startColumn, enum Direction direction);
	Move(int startRow, int startColumn, enum Direction direction, std::vector<Tile *>& tiles);
	Move(int startRow, int startColumn, enum Direction direction, std::vector<Tile *>& tiles, std::vector<wchar_t>& blankAssignment);

	int getStartRow();
	int getStartColumn();
	enum Direction getDirection();

	std::vector<Tile *>& getTiles();
	const std::vector<Tile *>& getTiles() const;

	std::vector<wchar_t>& getBlankAssignment();
	const std::vector<wchar_t>& getBlankAssignment() const;
};


#endif /* MOVE_H_ */
