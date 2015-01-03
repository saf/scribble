/*
 * Field.h
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#ifndef FIELD_H_
#define FIELD_H_

#include <vector>
#include "Tile.h"

typedef void (wordScoreFunc)(int &score);

class Field {
public:
	virtual ~Field() {};

	/* Fields may influence the score for a word in two ways:
	 *  - modifying the 'score' while the word is being traversed
	 *    to find per-letter scores - this will typically add the tile's value
	 *    plus any letter bonus.
	 *    This is realized by modifying the 'score' reference while this method is called.
	 *  - modifying the total score after the per-letter scores
	 *    are summed up - this will typically be a multiplicative word bonus.
	 *    This is realized by queueing a wordScoreFunc, which will be called after
	 *    this routine has been called for all fields.
	 */
	virtual void applyScore(Tile &tile, bool newTile, int &score, std::vector<wordScoreFunc *> &wordScoreFuncs) = 0;
};

#endif /* FIELD_H_ */
