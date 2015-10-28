#pragma once

#include <memory>
#include <iostream>
#include "Basics.h"
#include "Board.h"
#include "Grid.h"
#include "Tile.h"

class TilePlacement : public Grid<std::shared_ptr<Tile>> {
public:
	TilePlacement(const Board& board)
			: Grid(board.getHeight(), board.getWidth(), [](uint,uint){ return nullptr; }),
			  board_(board) {
	}

	TilePlacement(const TilePlacement& other) : Grid(other), board_(other.board_) {
	}

	const Board& getBoard() const {
		return board_;
	}

	bool checkMove(const Move& move) const;
	void applyMove(const Move& move);

	/// This returns a segment containing the given coordinates and
	/// any coordinates neighboring it in the given direction which have a tile on them.
	Segment<std::shared_ptr<Tile>> filledSegmentAround(Coordinates coordinates, Direction direction) const {
		Line<std::shared_ptr<Tile>> line = lineAt(coordinates, direction);
		uint length = 1;
		uint offset = coordinates.offsetOnLine(direction);
		for (int i = static_cast<int>(offset - 1); i >= 0 && line[i] != nullptr; i--) {
			offset--;
			length++;
		}
		for (uint i = offset + length; i < line.length() && line[i] != nullptr; i++) {
			length++;
		}
		return Segment<std::shared_ptr<Tile>>(line, offset, length);
	}
private:
	const Board& board_;
};
