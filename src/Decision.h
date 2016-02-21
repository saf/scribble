#pragma once

#include "Game.h"
#include "Move.h"

class GameState;

class Decision {
public:
	virtual ~Decision() {}

	virtual void applyToState(GameState& state) const = 0;

	virtual std::wstring toString() const = 0;
};

class MoveDecision : public Decision {
public:
	MoveDecision(Move move);

	void applyToState(GameState& state) const override;

	const Move& getMove() const;

	std::wstring toString() const override;
private:
	Move move_;
};

class TileExchangeDecision : public Decision {
public:
	TileExchangeDecision(Tiles exchangedTiles);

	void applyToState(GameState& state) const override;

	const Tiles& getExchangedTiles() const;

	std::wstring toString() const override;
private:
	const Tiles exchangedTiles_;
};

class PassDecision : public Decision {
	void applyToState(GameState& state) const override;

	std::wstring toString() const override;
};
