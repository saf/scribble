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

	void initializeTables(int height, int width);

	void putTile(int row, int col, Tile &tile);
	void removeTile(int row, int col);

	/** Get score for a single new word. This can be called in two variants:
	 *  * with tileIndex set to -1, this computes the score in the direction of the move.
	 *    [row] and [col] are coordinates of the first tile placed within the move.
	 *  * with tileIndex set to a value >= 0, this computes the score of the word orthogonal
	 *    to the actual move which is extended by this move. tileIndex in this case is set
	 *    to the index of the tile in [tiles] where the crossing takes place; [row] and [col]
	 *    are coordinates of this tile on the board.
	 */
	int getNewWordScore(std::vector<Tile *> *tiles, int row, int col, Move::Direction direction, int tileIndex = -1) const;
public:
	class InvalidMove : public std::exception {
	public:
		const char *what() {
			return "Board::applyMove called with an invalid move.";
		}
	};

	Board(int rows, int cols);
	Board(const Board &board);
	virtual ~Board();

	int getWidth() const;
	int getHeight() const;

	void setField(int row, int col, Field *field);
	const Field *getField(int row, int col) const;

	void setTile(int row, int col, Tile *tile);
	const Tile *getTile(int row, int col) const;

	bool checkMove(Move &move) const;
	int getMoveScore(Move &move) const;
	void applyMove(Move &move);
};

#endif /* BOARD_H_ */
