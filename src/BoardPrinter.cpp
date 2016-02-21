/*
 * BoardPrinter.cpp
 *
 *  Created on: 03-02-2015
 *      Author: saf
 */

#include "BoardPrinter.h"
#include <iostream>

BoardPrinter::BoardPrinter() {}

BoardPrinter::~BoardPrinter() {}

std::wstring BoardPrinter::getTileString(const std::shared_ptr<Tile>& tile,
		const BlankAssignments& blanks) {
	std::wstring s;
	if (tile != nullptr) {
		wchar_t letter = tile->isBlank() ? blanks.find(tile) : tile->getLetter();
		s.push_back(letter);
	} else {
		s = L" ";
	}
	return s;
}

std::wstring BoardPrinter::getFieldString(const Field& field,
		const std::shared_ptr<Tile>& tile, const BlankAssignments& blanks) {
	return BoardPrinter::getTileString(tile, blanks);
}

void BoardPrinter::printBoard(const Board& board,
		const TilePlacement& tiles, const BlankAssignments& blanks) {
	int width = board.getWidth();
	int height = board.getHeight();

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			std::wstring fieldString =
					getFieldString(board.getField(r, c), tiles[r][c], blanks);
			std::wcout << fieldString;
			if (c == width - 1) {
				if (r == width - 1) {
					std::wcout << std::endl;
				} else {
					std::wcout << std::endl << std::endl;
				}
			} else {
				std::wcout << L"   ";
			}
		}
	}
}
