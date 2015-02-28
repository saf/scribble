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

	if (hook.direction == UP) {
		std::vector<wchar_t> column = bi.columns[ctx.startCol];
		ctx.startRow--;
		while (ctx.startRow >= 0 && (ctx.startRow == 0 || bi.board.getTile(ctx.startRow - 1, ctx.startCol) == NULL)) {
			this->findMovesInRowOrCol(column, ctx.startRow, tiles, moveCheckingCallback, &ctx);
			ctx.startRow--;
		}
	} else {
		std::vector<wchar_t> row = bi.rows[ctx.startRow];
		ctx.startCol--;
		while (ctx.startCol >= 0 && (ctx.startCol == 0 || bi.board.getTile(ctx.startRow, ctx.startCol - 1) == NULL)) {
			this->findMovesInRowOrCol(row, ctx.startCol, tiles, moveCheckingCallback, &ctx);
		}
	}
}

void ESTree::findMovesSE(ESBoardInfo &bi, ESHook &hook, std::vector<Tile *> &tiles, void (*callback)(Move *move, void *context), void *context) {
	ESCallbackContext ctx = { callback, context, hook.row, hook.col, bi };

	if (hook.direction == DOWN) {
		std::vector<wchar_t> column = bi.columns[hook.col];
		this->findMovesInRowOrCol(column, hook.row, tiles, moveCheckingCallback, &ctx);
	} else {
		std::vector<wchar_t> row = bi.rows[hook.row];
		this->findMovesInRowOrCol(row, hook.col, tiles, moveCheckingCallback, &ctx);
	}
}

void ESTree::findMovesInRowOrCol(std::vector<wchar_t> &rowOrCol, int startPos, std::vector<Tile *> &tiles, void (*callback)(Move *, void *), void *context) {
	std::vector<bool> usedTiles(tiles.size());
	this->findMovesInSubtree(&this->root, rowOrCol, startPos, tiles, usedTiles, callback, context);
}

void ESTree::findMovesInSubtree(Node *node, std::vector<wchar_t> &rowOrCol, int startPos, std::vector<Tile *> &tiles, std::vector<bool> &usedTiles, void (*callback)(Move *, void *), void *context) {
	for (int i = 0; i < tiles.size(); i++) {
		if (!usedTiles[i]) {
			Tile *tile = tiles[i];
			int index = alphabet->getIndex(tile->getLetter());
			Node *child = node->find(index);
			if (child != NULL) {
				usedTiles[i] = true;
				this->findMovesInSubtree(child, rowOrCol, startPos + 1, tiles, usedTiles, callback, context);
				usedTiles[i] = false;
			}
		}
	}
	/* #TODO existing letters on the board, handle blanks, etc. */
}
