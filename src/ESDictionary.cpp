/*
 * ESDictionary.cpp
 *
 *  Created on: 18-02-2015
 *      Author: saf
 */

#include "ESDictionary.h"

ESDictionary::ESDictionary(Alphabet &alphabet) : Dictionary(alphabet) {
	delete this->tree; /* Delete base-class Trie created by our base class. */
	this->tree = new ESTree(alphabet);
}

ESDictionary::~ESDictionary() {
	delete this->tree;
}

void ESDictionary::findMovesAtHook(ESBoardInfo &bi, ESHook &hook, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context) {
	tree->findMovesAtHook(bi, hook, tiles, callback, context);
}
