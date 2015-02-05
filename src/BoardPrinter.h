/*
 * BoardPrinter.h
 *
 *  Created on: 03-02-2015
 *      Author: saf
 */

#ifndef BOARDPRINTER_H_
#define BOARDPRINTER_H_

#include "Board.h"

class BoardPrinter {
protected:
	virtual std::wstring getTileString(const Tile *tile);
	virtual std::wstring getFieldString(const Field *field, const Tile *tile);
public:
	BoardPrinter();
	virtual ~BoardPrinter();

	virtual void printBoard(const Board &board);
};

#endif /* BOARDPRINTER_H_ */
