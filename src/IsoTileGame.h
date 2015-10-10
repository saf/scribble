/*
 * IsoTileGame.h
 *
 * This represents the group of games for which
 * any two tiles of the same letter are identical.
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#ifndef ISOTILEGAME_H_
#define ISOTILEGAME_H_

#include "Game.h"
#include "Move.h"

class IsoTileGame : public Game {
public:
	struct TileGroup {
		int multiplicity; /* Number of tiles of this kind in the bag */
		wchar_t letter;
		int points;
		int color;
	};
protected:

	IsoTileGame(std::vector<std::unique_ptr<Player>> players);
	virtual ~IsoTileGame() {};

	IsoTileGame(const IsoTileGame&) = delete;
	IsoTileGame(IsoTileGame&& other);

	IsoTileGame& operator=(const IsoTileGame&) = delete;
	IsoTileGame& operator=(IsoTileGame&& other);

	virtual std::vector<TileGroup> getTileGroups() = 0;

	Tileset getInitialBag();

	static Tileset findTilesForPlayerRack(const GameState& state, const wchar_t* rackLetters);
	static Tiles findTilesForPlayerMove(const GameState& state, int row, int column, Move::Direction direction, const wchar_t* wordLetters);
};

#endif /* ISOTILEGAME_H_ */
