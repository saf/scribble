/*
 * Board.h
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#ifndef BOARD_H_
#define BOARD_H_

class Board {
private:
	Field **fields;
public:
	Board(int rows, int cols);
	virtual ~Board();
};

#endif /* BOARD_H_ */
