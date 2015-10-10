#pragma once

#include "Game.h"
#include "Move.h"

class GameState;

class Decision {
public:
	virtual ~Decision() {}

	virtual void applyToState(GameState& state) const = 0;
};

class MoveDecision : public Decision {
public:
	MoveDecision(Move move);

	void applyToState(GameState& state) const override;

	const Move& getMove() const;
private:
	Move move_;
};

class TileExchangeDecision : public Decision {
public:
	TileExchangeDecision(Tileset exchangedTiles);

	void applyToState(GameState& state) const override;

	const Tileset& getExchangedTiles() const;
private:
	const Tileset exchangedTiles_;
};

class PassDecision : public Decision {
	void applyToState(GameState& state) const override;
};
