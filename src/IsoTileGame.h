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

	Tileset getInitialBag() override;

	template<typename C>
	static std::shared_ptr<Tile> extractTile(C& tileCollection, Letter letter) {
		auto it = tileCollection.begin();
		while (it != tileCollection.end()) {
			std::shared_ptr<Tile> tile = *it;
			if ((letter == LETTER('_') && tile->isBlank()) || tile->getLetter() == letter) {
				tileCollection.erase(it);
				return tile;
			}
			++it;
		}
		return std::shared_ptr<Tile>(nullptr);
	}
};

#endif /* ISOTILEGAME_H_ */
