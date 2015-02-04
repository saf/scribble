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
	static const int RACK_SIZE;

	/* Keep fields allocated by us so that we can delete them when we are destroyed. */
	std::vector<Field *> myFields;
protected:
	const struct TileGroup *getTileGroups();
	int getTileGroupCount();

	Board getInitialBoard();
public:
	Literaki(std::vector<Player *> players);
	virtual ~Literaki();

	int getRackSize();
};

#endif /* LITERAKI_H_ */
