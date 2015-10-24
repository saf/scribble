#include "TilePlacement.h"

void TilePlacement::applyMove(const Move& move) {
	Direction direction = move.getDirection();
	int row = move.getStartRow();
	int col = move.getStartColumn();
	const Tiles& tiles = move.getTiles();

	if (!this->checkMove(move)) {
		throw InvalidMove();
	}

	while (at(row).at(col) != nullptr) {
		if (direction == Direction::HORIZONTAL) {
			col++;
		} else {
			row++;
		}
	}

	for (const std::shared_ptr<Tile>& tile : tiles) {
		at(row)[col] = tile;

		if (direction == Direction::HORIZONTAL) {
			col++;
			while (col < this->columnCount_ && at(row).at(col) != nullptr) {
				col++;
			}
		} else {
			row++;
			while (row < this->rowCount_ && at(row).at(col) != nullptr) {
				row++;
			}
		}
	}
}

bool TilePlacement::checkMove(const Move& move) const {
	Direction direction = move.getDirection();
	int startRow = move.getStartRow();
	int startColumn = move.getStartColumn();
	const Tiles& tiles = move.getTiles();
	int row = startRow;
	int col = startColumn;

	if (row < 0 || col < 0 || row >= this->rowCount_ || col >= this->columnCount_) {
		return false;
	}

	while (row < this->rowCount_ && col < this->columnCount_ && at(row).at(col) != nullptr) {
		if (direction == Direction::HORIZONTAL) {
			col++;
		} else {
			row++;
		}
	}

	for (const std::shared_ptr<Tile>& tile : tiles) {
		if (row >= this->rowCount_ || col >= this->columnCount_) {
			return false;
		}

		if (direction == Direction::HORIZONTAL) {
			col++;
		} else {
			row++;
		}

		while (row < this->rowCount_ && col < this->columnCount_ && at(row).at(col) != nullptr) {
			if (direction == Direction::HORIZONTAL) {
				col++;
			} else {
				row++;
			}
		}
	}

	return true;
}
