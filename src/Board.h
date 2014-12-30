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
#include "Tile.h"

class Board {
private:
	int width;  /* number of columns */
	int height; /* number of rows */

	std::vector<std::vector<Field *> > fields;
	std::vector<std::vector<Tile *> > tiles;

	std::vector< std::vector<wchar_t> > rows;
	std::vector< std::vector<wchar_t> > columns;

	void putTile(int row, int col, Tile &tile);
	void removeTile(int row, int col);

	int getNewWordScore(std::vector<Tile *> *tiles, int row, int col, Move::Direction direction, int tileIndex = -1);
public:
	Board(int rows, int cols);
	Board(const Board &board);
	virtual ~Board();

	void setField(int row, int col, Field *field);
	Field *getField(int row, int col);

	bool checkMove(Move &move);
	int getMoveScore(Move &move);
	void applyMove(Move &move);
};

#endif /* BOARD_H_ */
