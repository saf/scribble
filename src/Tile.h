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
private:
	const wchar_t letter;
	const int color;
	const int points;
public:
	Tile(wchar_t l, int p, int c = 0) : letter(l), points(p), color(c) {};

	wchar_t getLetter() const { return letter; }
	int getColor() const { return color; }
	int getPoints() const { return points; }
};

#endif /* TILE_H_ */
