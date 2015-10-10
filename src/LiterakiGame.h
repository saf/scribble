/*
 * Literaki.h
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#ifndef LITERAKI_H_
#define LITERAKI_H_

#include "IsoTileGame.h"

class LiterakiGame : public IsoTileGame {

public:
	enum Colors { YELLOW, GREEN, BLUE, RED };

	LiterakiGame(std::vector<std::unique_ptr<Player>> players);
	virtual ~LiterakiGame() {};

	LiterakiGame(const LiterakiGame&) = delete;
	LiterakiGame(LiterakiGame&& other);

	LiterakiGame& operator=(const LiterakiGame&) = delete;
	LiterakiGame& operator=(LiterakiGame&& other);

	int getRackSize() const;

	static LiterakiGame readFromStream(std::wistream &s);

protected:
	std::vector<TileGroup> getTileGroups();

	Board getInitialBoard();

private:
	static const int RACK_SIZE;
};

#endif /* LITERAKI_H_ */
