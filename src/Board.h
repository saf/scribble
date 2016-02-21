/*
 * Board.h
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <vector>

#include "Move.h"
#include "Field.h"
#include "Grid.h"
#include "Tile.h"

class InvalidMove : public std::exception {
public:
	const char *what() {
		return "Board::applyMove called with an invalid move.";
	}
};

class Board {
public:
	Board(int rows, int cols, std::function<std::unique_ptr<Field>(uint, uint)> fieldInitializer);

	Board(const Board &board) = delete;
	Board(Board&& other);
	virtual ~Board();

	Board& operator=(const Board&) = delete;
	Board& operator=(Board&& other);

	int getWidth() const;
	int getHeight() const;

	const Field& getField(const Coordinates& coords) const;
	const Field& getField(uint row, uint col) const;

private:
	Grid<std::unique_ptr<Field>> fields;
	std::vector<std::vector<Tile *> > tiles;
};

#endif /* BOARD_H_ */
