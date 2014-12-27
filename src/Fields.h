/*
 * Fields.h
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#ifndef FIELDS_H_
#define FIELDS_H_

#include "Field.h"

class PlainField : public Field {
public:
	virtual void applyScore(Tile &tile, int &score, std::vector<wordScoreFunc *> &wordScoreFuncs) {
		score += tile.getPoints();
	}
};

template <int bonus> class MultiplicativeLetterBonusField : public Field {
public:
	virtual void applyScore(Tile &tile, int &score, std::vector<wordScoreFunc *> &wordScoreFuncs) {
		score += tile.getPoints() * bonus;
	}
};

template <int factor> class MultiplicativeWordBonusField : public Field {
private:
	void multiplyWordScore(int &score) {
		score *= factor;
	}
public:
	virtual void applyScore(Tile &tile, int &score, std::vector<wordScoreFunc *> &wordScoreFuncs) {
		score += tile.getPoints();
		wordScoreFuncs.push_back(multiplyWordScore);
	}
};

template <int color, int factor> class ColoredField : public Field {
public:
	virtual void applyScore(Tile &tile, int &score, std::vector<wordScoreFunc *> &wordScoreFuncs) {
		if (tile.getColor() == color) {
			score += factor * tile.getPoints();
		} else {
			score += tile.getPoints();
		}
	}
};

#endif /* FIELDS_H_ */
