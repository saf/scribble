/*
 * Literaki.h
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#ifndef LITERAKI_H_
#define LITERAKI_H_

#include "IsoTileGame.h"

class Literaki : public IsoTileGame {

public:
	enum Colors { YELLOW, GREEN, BLUE, RED };
private:
	static const struct TileGroup *tileGroups;
	static const int tileGroupCount;
protected:
	const struct TileGroup *getTileGroups();
	int getTileGroupCount();

	Board getInitialBoard();
public:
	Literaki(std::vector<Player *> players);
	virtual ~Literaki();
};

#endif /* LITERAKI_H_ */
