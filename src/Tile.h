/*
 * Tile.h
 *
 * Represents a single tile in the game
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#ifndef TILE_H_
#define TILE_H_

class Tile {
protected:
	const wchar_t letter;
	const int color;
	const int points;
public:
	Tile(wchar_t l, int p, int c = 0) : letter(l), points(p), color(c) {};
	virtual ~Tile() {};

	virtual wchar_t getLetter() const { return letter; }
	int getColor() const { return color; }
	int getPoints() const { return points; }
	virtual bool isBlank() const { return false; }
};

class BlankTile : public Tile {
protected:
	wchar_t filledLetter;
public:
	virtual ~BlankTile() {};
	BlankTile() : Tile(L'_', 0, -1), filledLetter(L' ') {};
	virtual wchar_t getLetter() const { return filledLetter == L' ' ? L'_' : filledLetter; };
	void fillLetter(wchar_t letter) { this->filledLetter = letter; };
	virtual bool isBlank() const { return true; }
};

#endif /* TILE_H_ */
