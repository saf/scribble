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

class FieldVisitor;

class Field {
public:
	virtual ~Field() {};

	/* Fields may influence the score for a word in two ways:
	 *  - modifying the 'score' while the word is being traversed
	 *    to find per-letter scores - this will typically add the tile's value
	 *    plus any letter bonus.
	 *    This is realized by modifying the 'score' reference while this method is called.
	 *  TODO doc
	 */
	virtual void applyScore(const Tile &tile, bool newTile, int &score) const = 0;

	virtual void changeWordScore(bool newTile, int &score) const {};

	virtual void accept(FieldVisitor &v) const = 0;
};

#endif /* FIELD_H_ */
