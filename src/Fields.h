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
	void applyScore(Tile &tile, bool newTile, int &score) {
		score += tile.getPoints();
	}
};

class MultiplicativeLetterBonusField : public Field {
private:
	int factor;
public:
	MultiplicativeLetterBonusField(int factor) {
		this->factor = factor;
	}
	void applyScore(Tile &tile, bool newTile, int &score) {
		score += tile.getPoints() * (newTile ? factor : 1);
	}
};

class MultiplicativeWordBonusField : public Field {
private:
	int factor;
public:
	MultiplicativeWordBonusField(int factor) {
		this->factor = factor;
	}
	void applyScore(Tile &tile, bool newTile, int &score) {
		score += tile.getPoints();
	}
	void changeWordScore(bool newTile, int &score) {
		if (newTile) {
			score *= this->factor;
		}
	}
};

class ColoredField : public Field {
private:
	int factor;
	int color;
public:
	ColoredField(int factor, int color) {
		this->factor = factor;
		this->color = color;
	}
	void applyScore(Tile &tile, bool newTile, int &score) {
		if (newTile && tile.getColor() == this->color) {
			score += this->factor * tile.getPoints();
		} else {
			score += tile.getPoints();
		}
	}
};

#endif /* FIELDS_H_ */
