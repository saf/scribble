/*
 * ESTree.h
 *
 *  Created on: 18-02-2015
 *      Author: saf
 */

#ifndef ESTREE_H_
#define ESTREE_H_

#include "Trie.h"
#include "Dictionary.h"
#include "Board.h"
#include "Move.h"

enum ESHookDirection { LEFT, RIGHT, UP, DOWN };
struct ESHook {
	int row;
	int col;
	enum ESHookDirection direction;
};

struct ESBoardInfo {
	const Board &board;
	const std::vector<std::vector<wchar_t> >& rows;
	const std::vector<std::vector<wchar_t> >& columns;

	ESBoardInfo(const Board& b, const std::vector<std::vector<wchar_t> >& r, const std::vector<std::vector<wchar_t> >& c) : board(b), rows(r), columns(c) {};
};

struct ESCallbackContext {
	void (*callerCallback)(Move *, void *);
	void *callerContext;
	int startRow;
	int startCol;
	ESBoardInfo &boardInfo;
};

class ESTree : public Trie {
private:
	void findMovesNW(ESBoardInfo &bi, ESHook &hook, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context);
	void findMovesSE(ESBoardInfo &bi, ESHook &hook, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context);
	void findMovesInRowOrCol(std::vector<wchar_t> &rowOrCol, int startPos, Move *partialMove, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context);
	void findMovesInSubtree(Node* node, std::vector<wchar_t> &rowOrCol, size_t startPos, Move *partialMove, std::vector<Tile *> &tiles, std::vector<bool> &usedTiles, void (*callback)(Move *, void *), void *context);
public:
	ESTree(Alphabet &alphabet);
	virtual ~ESTree();

	void findMovesAtHook(ESBoardInfo &bi, ESHook &hook, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context);
};

#endif /* ESTREE_H_ */
