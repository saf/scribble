/*
 * ESDictionary.h
 *
 *  Created on: 18-02-2015
 *      Author: saf
 */

#ifndef ESDICTIONARY_H_
#define ESDICTIONARY_H_

#include "Dictionary.h"
#include "ESTree.h"

class ESDictionary : public Dictionary {
protected:
	ESTree *tree;
public:
	ESDictionary(Alphabet &alphabet);
	virtual ~ESDictionary();

	void findMovesAtHook(ESBoardInfo &bi, ESHook &hook, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context);
};

#endif /* ESDICTIONARY_H_ */
