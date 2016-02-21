/*
 * BoardPrinter.h
 *
 *  Created on: 03-02-2015
 *      Author: saf
 */

#ifndef BOARDPRINTER_H_
#define BOARDPRINTER_H_

#include "Board.h"
#include "TilePlacement.h"

class BoardPrinter {
protected:
	virtual std::wstring getTileString(const std::shared_ptr<Tile>& tile,
			const BlankAssignments& blanks);
	virtual std::wstring getFieldString(const Field& field,
			const std::shared_ptr<Tile>& tile, const BlankAssignments& blanks);
public:
	BoardPrinter();
	virtual ~BoardPrinter();

	virtual void printBoard(const Board& board,
			const TilePlacement& tiles, const BlankAssignments& blanks);
};

#endif /* BOARDPRINTER_H_ */
