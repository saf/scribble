#include "TilePlacement.h"

void TilePlacement::applyMove(const Move& move) {
	Direction direction = move.getDirection();
	Coordinates startCoords = move.getCoordinates();
	const Tiles& tiles = move.getTiles();
	auto segment = segmentFrom(startCoords, direction);

	if (!this->checkMove(move)) {
		throw InvalidMove();
	}

	uint index = 0;
	for (auto tile : tiles) {
		while (index < segment.length() && segment[index] != nullptr) {
			index++;
		}
		if (index < segment.length()) {
			atCoords(segment.coordinates(index)) = tile;
			index++;

		} else {
			throw InvalidMove();
		}
	}
}

bool TilePlacement::checkMove(const Move& move) const {
	Direction direction = move.getDirection();
	Coordinates startCoords = move.getCoordinates();
	const Tiles& tiles = move.getTiles();

	if (startCoords.row < 0 || startCoords.column < 0
			|| startCoords.row >= this->rowCount_ || startCoords.column >= this->columnCount_) {
		return false;
	}
	auto segment = segmentFrom(startCoords, direction);

	uint index = 0;
	for (auto tile : tiles) {
		while (index < segment.length() && segment[index] != nullptr) {
			index++;
		}
		if (index >= segment.length()) {
			return false;
		}
		index++;
	}

	return true;
}
