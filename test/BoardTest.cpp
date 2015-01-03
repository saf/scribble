#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Board
#include <boost/test/unit_test.hpp>
#include <cstdio>

#include "Board.h"
#include "Fields.h"

struct MoveManagementTestSetup {
	Board b;
	std::vector<Tile *> startTiles;
	std::vector<Tile *> zaTiles;

	MoveManagementTestSetup() : b(5, 4) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				b.setField(i, j, new PlainField());
			}
		}

		startTiles.push_back(new Tile(L'D', 2));
		startTiles.push_back(new Tile(L'Ą', 5));
		startTiles.push_back(new Tile(L'Ł', 3));

		zaTiles.push_back(new Tile(L'Z', 2));
		zaTiles.push_back(new Tile(L'E', 1));
	}
};

BOOST_FIXTURE_TEST_SUITE(checkFirstMove, MoveManagementTestSetup);

BOOST_AUTO_TEST_CASE(checkMoveLeftOfBoard) {
	Move m(0, -1, Move::HORIZONTAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == false);
}

BOOST_AUTO_TEST_CASE(checkMoveAboveBoard) {
	Move m(-1, 0, Move::HORIZONTAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == false);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBoardTopBorder) {
	Move m(0, 1, Move::HORIZONTAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBoardLeftBorder) {
	Move m(1, 0, Move::VERTICAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBoardLastRow) {
	Move m(3, 0, Move::HORIZONTAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBoardLastColumn) {
	Move m(0, 3, Move::VERTICAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtRightBorder) {
	Move m(0, 1, Move::HORIZONTAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBottomBorder) {
	Move m(2, 0, Move::VERTICAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveOverRightBorder) {
	Move m(0, 2, Move::HORIZONTAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == false);
}

BOOST_AUTO_TEST_CASE(checkMoveOverBottomBorder) {
	Move m(3, 0, Move::VERTICAL, &startTiles);
	BOOST_CHECK(b.checkMove(m) == false);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(firstMovePlacement, MoveManagementTestSetup);

BOOST_AUTO_TEST_CASE(horizontalWordAtTopLeftCorner) {
	Move m(0, 0, Move::HORIZONTAL, &startTiles);
	b.applyMove(m);
	for (int i = 0; i < startTiles.size(); i++) {
		BOOST_CHECK_EQUAL(b.getTile(0, i)->getLetter(), startTiles[i]->getLetter());
	}
}

BOOST_AUTO_TEST_CASE(verticalWordAtTopLeftCorner) {
	Move m(0, 0, Move::VERTICAL, &startTiles);
	b.applyMove(m);
	for (int i = 0; i < startTiles.size(); i++) {
		BOOST_CHECK_EQUAL(b.getTile(i, 0)->getLetter(), startTiles[i]->getLetter());
	}
}

BOOST_AUTO_TEST_CASE(horizontalWordAtBottomRightCorner) {
	Move m(4, 1, Move::HORIZONTAL, &startTiles);
	b.applyMove(m);
	for (int i = 0; i < startTiles.size(); i++) {
		BOOST_CHECK_EQUAL(b.getTile(4, 1 + i)->getLetter(), startTiles[i]->getLetter());
	}
}

BOOST_AUTO_TEST_CASE(verticalWordAtBottomRightCorner) {
	Move m(2, 3, Move::VERTICAL, &startTiles);
	b.applyMove(m);
	for (int i = 0; i < startTiles.size(); i++) {
		BOOST_CHECK_EQUAL(b.getTile(2 + i, 3)->getLetter(), startTiles[i]->getLetter());
	}
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(checkSecondMove, MoveManagementTestSetup);

BOOST_AUTO_TEST_SUITE_END();

