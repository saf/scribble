/*
 * ESTree.cpp
 *
 *  Created on: 18-02-2015
 *      Author: saf
 */

#include "ESTree.h"

ESTree::ESTree(Alphabet &alphabet) : Trie(alphabet) {}
ESTree::~ESTree() {}

static bool checkOrthogonalWords(ESBoardInfo &bi, int row, int col, Move *move) {
	return true;
}

static void moveCheckingCallback(Move *move, void *context) {
	ESCallbackContext *ctx = static_cast<ESCallbackContext *>(context);
	if (checkOrthogonalWords(ctx->boardInfo, ctx->startRow, ctx->startCol, move)) {
		(ctx->callerCallback)(move, ctx->callerContext);
	}
}

void ESTree::findMovesAtHook(ESBoardInfo &bi, ESHook &hook, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context) {

	switch (hook.direction) {
	case UP:
	case LEFT:
		this->findMovesNW(bi, hook, tiles, callback, context);
		break;
	case DOWN:
	case RIGHT:
		this->findMovesSE(bi, hook, tiles, callback, context);
	}
}

void ESTree::findMovesNW(ESBoardInfo &bi, ESHook &hook, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context) {
	ESCallbackContext ctx = { callback, context, hook.row, hook.col, bi };
	std::vector<Tile *> moveTiles;

	if (hook.direction == UP) {
		std::vector<wchar_t> column = bi.columns[ctx.startCol];
		ctx.startRow--;
		while (ctx.startRow >= 0 && (ctx.startRow == 0 || bi.board.getTile(ctx.startRow - 1, ctx.startCol) == NULL)) {
			Move moveTemplate(ctx.startRow, ctx.startCol, Move::VERTICAL, moveTiles);
			this->findMovesInRowOrCol(column, ctx.startRow, &moveTemplate, tiles, moveCheckingCallback, &ctx);
			ctx.startRow--;
		}
	} else {
		std::vector<wchar_t> row = bi.rows[ctx.startRow];
		ctx.startCol--;
		while (ctx.startCol >= 0 && (ctx.startCol == 0 || bi.board.getTile(ctx.startRow, ctx.startCol - 1) == NULL)) {
			Move moveTemplate(ctx.startRow, ctx.startCol, Move::HORIZONTAL, moveTiles);
			this->findMovesInRowOrCol(row, ctx.startCol, &moveTemplate, tiles, moveCheckingCallback, &ctx);
		}
	}
}

void ESTree::findMovesSE(ESBoardInfo &bi, ESHook &hook, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context) {
//	ESCallbackContext ctx = { callback, context, hook.row, hook.col, bi };
//	std::vector<Tile *> *moveTiles;

//	if (hook.direction == DOWN) {
//		Move moveTemplate(ctx.startRow, ctx.startCol, Move::VERTICAL, *moveTiles);
//		std::vector<wchar_t> column = bi.columns[hook.col];
//		this->findMovesInRowOrCol(column, hook.row, &moveTemplate, tiles, moveCheckingCallback, &ctx);
//	} else {
//		Move moveTemplate(ctx.startRow, ctx.startCol, Move::HORIZONTAL, *moveTiles);
//		std::vector<wchar_t> row = bi.rows[hook.row];
//		this->findMovesInRowOrCol(row, hook.col, &moveTemplate, tiles, moveCheckingCallback, &ctx);
//	}
}

void ESTree::findMovesInRowOrCol(std::vector<wchar_t> &rowOrCol, int startPos, Move *partialMove, std::vector<Tile *> &tiles, void (*callback)(Move *, void *), void *context) {
	std::vector<bool> usedTiles(tiles.size());
	this->findMovesInSubtree(&this->root_, rowOrCol, startPos, partialMove, tiles, usedTiles, callback, context);
}

void ESTree::findMovesInSubtree(Node* node, std::vector<wchar_t> &rowOrCol, size_t startPos, Move *partialMove, std::vector<Tile *> &tiles, std::vector<bool> &usedTiles, void (*callback)(Move *, void *), void *context) {

	if (startPos < rowOrCol.size() && rowOrCol[startPos] != LETTER('.')) {
		const std::unique_ptr<Node>& child = node->find(alphabet_.getIndex(rowOrCol[startPos]));
		if (child != nullptr) {
			findMovesInSubtree(child.get(), rowOrCol, startPos, partialMove, tiles, usedTiles, callback, context);
		}
	} else {
		if (node->isFinal()) {
			callback(partialMove, context);
		}

		if (startPos < rowOrCol.size()) {
			for (size_t i = 0; i < tiles.size(); i++) {
				if (!usedTiles[i]) {
					Tile *tile = tiles[i];
					usedTiles[i] = true;
					partialMove->getTiles().push_back(tile);

					if (tile->isBlank()) {
						// BlankTile *blank = static_cast<BlankTile *>(tile);
						// TODO Blank tiles should probably disappear from the bag and be replaced on the board by brand new 0-valued tiles.
						std::vector<wchar_t>& blankAssignment = partialMove->getBlankAssignment();
						for (size_t li = 0; li < alphabet_.getLetterCount(); li++) {
							const std::unique_ptr<Node>& child = node->find(li);
							blankAssignment.push_back(alphabet_.getLetter(li));
							findMovesInSubtree(child.get(), rowOrCol, startPos + 1, partialMove, tiles, usedTiles, callback, context);
							blankAssignment.pop_back();
						}
					} else {
						int index = alphabet_.getIndex(tile->getLetter());
						const std::unique_ptr<Node>& child = node->find(index);
						if (child != nullptr) {
							this->findMovesInSubtree(child.get(), rowOrCol, startPos + 1, partialMove, tiles, usedTiles, callback, context);
						}
					}

					partialMove->getTiles().pop_back();
					usedTiles[i] = false;
				}
			}
		}
	}
}
