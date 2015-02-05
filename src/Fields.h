/*
 * Fields.h
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#ifndef FIELDS_H_
#define FIELDS_H_

#include "Field.h"
#include <cstdio>

class PlainField;
class MultiplicativeLetterBonusField;
class MultiplicativeWordBonusField;
class ColoredField;

class FieldVisitor {
public:
	virtual ~FieldVisitor() {};
	virtual void visit(const PlainField *f) = 0;
	virtual void visit(const MultiplicativeLetterBonusField *f) = 0;
	virtual void visit(const MultiplicativeWordBonusField *f) = 0;
	virtual void visit(const ColoredField *f) = 0;
};

class PlainField : public Field {
public:
	void applyScore(Tile &tile, bool newTile, int &score) {
		score += tile.getPoints();
	}
	void accept(FieldVisitor &v) const {
		v.visit(this);
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
	void accept(FieldVisitor &v) const {
		v.visit(this);
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
	int getFactor() const {
		return this->factor;
	}
	virtual void accept(FieldVisitor &v) const {
		v.visit(this);
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
	int getColor() const {
		return this->color;
	}
	virtual void accept(FieldVisitor &v) const {
		v.visit(this);
	}
};

#endif /* FIELDS_H_ */
