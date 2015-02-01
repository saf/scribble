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

class IsoTileGame : public Game {
public:
	struct TileGroup {
		int multiplicity; /* Number of tiles of this kind in the bag */
		wchar_t letter;
		int points;
		int color;
	};
private:
	/* Preserve pointers to tiles allocated by us so that they can be freed
	 * when we are destroyed. */
	std::vector<Tile *> myTiles;
protected:

	IsoTileGame(std::vector<Player *> &players);
	virtual ~IsoTileGame();

	virtual const IsoTileGame::TileGroup *getTileGroups() = 0;
	virtual int getTileGroupCount() = 0;

	std::set<Tile *> getInitialBag();
};

#endif /* ISOTILEGAME_H_ */
