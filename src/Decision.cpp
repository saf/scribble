#include "Decision.h"
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

TileExchangeDecision::TileExchangeDecision(Tileset exchangedTiles)
		: exchangedTiles_(std::move(exchangedTiles)) {
}

void TileExchangeDecision::applyToState(GameState& state) const {
	state.applyExchangeDecision(*this);
}

const Tileset& TileExchangeDecision::getExchangedTiles() const {
	return exchangedTiles_;
}

void PassDecision::applyToState(GameState& state) const {
	state.applyPassDecision(*this);
}
