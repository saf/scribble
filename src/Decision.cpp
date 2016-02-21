#include "Decision.h"

#include <sstream>

#include "GameState.h"

MoveDecision::MoveDecision(Move move)
	: move_(move) {
}

void MoveDecision::applyToState(GameState& state) const {
	state.applyMoveDecision(*this);
}

const Move& MoveDecision::getMove() const {
	return move_;
}

std::wstring MoveDecision::toString() const {
	std::wostringstream s;
	s << "Move (" << move_.getCoordinates().row << ", "
			<< move_.getCoordinates().column << ") ";
	if (move_.getDirection() == Direction::HORIZONTAL) {
		s << "→ ";
	} else {
		s << "↓ ";
	}

	for (auto& tile : move_.getTiles()) {
		if (tile->isBlank()) {
			s << "[" << move_.getBlankAssignments().find(tile) << "]";
		} else {
			s << tile->getLetter();
		}
	}
	return s.str();
}

TileExchangeDecision::TileExchangeDecision(Tiles exchangedTiles)
		: exchangedTiles_(std::move(exchangedTiles)) {
}

void TileExchangeDecision::applyToState(GameState& state) const {
	state.applyExchangeDecision(*this);
}

std::wstring TileExchangeDecision::toString() const {
	std::wostringstream s;
	s << "Exchange ";
	for (auto& tile : exchangedTiles_) {
		s << tile->getLetter();
	}
	return s.str();
}

const Tiles& TileExchangeDecision::getExchangedTiles() const {
	return exchangedTiles_;
}

void PassDecision::applyToState(GameState& state) const {
	state.applyPassDecision(*this);
}

std::wstring PassDecision::toString() const {
	return WORD("Pass");
}
