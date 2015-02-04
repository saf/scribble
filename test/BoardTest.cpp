#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Board
#include <boost/test/unit_test.hpp>
#include <cstdio>

#include "Board.h"
#include "Fields.h"

enum colors { PLAIN, YELLOW, GREEN, BLUE, RED };

struct MoveManagementTestSetup {
	Board b;
	std::vector<Tile *> startTiles;
	std::vector<Tile *> zaTiles;

	MoveManagementTestSetup() : b(5, 4) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				b.setField(i, j, new PlainField());
			}
		}

		startTiles.push_back(new Tile(L'D', 2, GREEN));
		startTiles.push_back(new Tile(L'Ą', 5, RED));
		startTiles.push_back(new Tile(L'Ł', 3, BLUE));

		zaTiles.push_back(new Tile(L'Z', 2, YELLOW));
		zaTiles.push_back(new Tile(L'A', 1, YELLOW));
	}

	~MoveManagementTestSetup() {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				delete b.getField(i, j);
			}
		}
		while (!startTiles.empty()) {
			Tile *t = startTiles.back();
			startTiles.pop_back();
			delete t;
		}
		while (!zaTiles.empty()) {
			Tile *t = zaTiles.back();
			zaTiles.pop_back();
			delete t;
		}
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

BOOST_AUTO_TEST_CASE(checkVerticalCrossInsideBoard) {
	Move m1(1, 0, Move::HORIZONTAL, &startTiles);
	Move m2(0, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(b.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkVerticalAddTopInsideBoard) {
	Move m1(2, 0, Move::HORIZONTAL, &startTiles);
	Move m2(0, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(b.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkVerticalAddBottomInsideBoard) {
	Move m1(0, 0, Move::HORIZONTAL, &startTiles);
	Move m2(1, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(b.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkVerticalCrossOutsideBoard) {
	Move m1(4, 0, Move::HORIZONTAL, &startTiles);
	Move m2(3, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(!b.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkVerticalAddTopAtBorder) {
	Move m1(4, 0, Move::HORIZONTAL, &startTiles);
	Move m2(2, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(b.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalCrossInsideBoard) {
	Move m1(0, 1, Move::VERTICAL, &startTiles);
	Move m2(0, 0, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(b.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalAddLeftInsideBoard) {
	Move m1(0, 2, Move::VERTICAL, &startTiles);
	Move m2(0, 0, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(b.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalAddRightInsideBoard) {
	Move m1(0, 0, Move::VERTICAL, &startTiles);
	Move m2(0, 1, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(b.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalCrossOutsideBoard) {
	Move m1(0, 3, Move::VERTICAL, &startTiles);
	Move m2(0, 2, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(!b.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalAddLeftAtBorder) {
	Move m1(0, 3, Move::VERTICAL, &startTiles);
	Move m2(0, 1, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK(b.checkMove(m2));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(secondMovePlacement, MoveManagementTestSetup);

BOOST_AUTO_TEST_CASE(verticalCrossInsideBoard) {
	Move m1(1, 0, Move::HORIZONTAL, &startTiles);
	Move m2(0, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	b.applyMove(m2);
	BOOST_CHECK(b.getTile(0, 0)->getLetter() == L'Z');
	BOOST_CHECK(b.getTile(2, 0)->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(verticalAddTopInsideBoard) {
	Move m1(2, 0, Move::HORIZONTAL, &startTiles);
	Move m2(0, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	b.applyMove(m2);
	BOOST_CHECK(b.getTile(0, 0)->getLetter() == L'Z');
	BOOST_CHECK(b.getTile(1, 0)->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(verticalAddBottomInsideBoard) {
	Move m1(0, 0, Move::HORIZONTAL, &startTiles);
	Move m2(1, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	b.applyMove(m2);
	BOOST_CHECK(b.getTile(1, 0)->getLetter() == L'Z');
	BOOST_CHECK(b.getTile(2, 0)->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(verticalCrossOutsideBoard) {
	Move m1(4, 0, Move::HORIZONTAL, &startTiles);
	Move m2(3, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK_THROW(b.applyMove(m2), Board::InvalidMove);
}

BOOST_AUTO_TEST_CASE(verticalAddTopAtBorder) {
	Move m1(4, 0, Move::HORIZONTAL, &startTiles);
	Move m2(2, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	b.applyMove(m2);
	BOOST_CHECK(b.getTile(2, 0)->getLetter() == L'Z');
	BOOST_CHECK(b.getTile(3, 0)->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(horizontalCrossInsideBoard) {
	Move m1(0, 1, Move::VERTICAL, &startTiles);
	Move m2(0, 0, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	b.applyMove(m2);
	BOOST_CHECK(b.getTile(0, 0)->getLetter() == L'Z');
	BOOST_CHECK(b.getTile(0, 2)->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(horizontalAddLeftInsideBoard) {
	Move m1(0, 2, Move::VERTICAL, &startTiles);
	Move m2(0, 0, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	b.applyMove(m2);
	BOOST_CHECK(b.getTile(0, 0)->getLetter() == L'Z');
	BOOST_CHECK(b.getTile(0, 1)->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(horizontalAddRightInsideBoard) {
	Move m1(0, 0, Move::VERTICAL, &startTiles);
	Move m2(0, 1, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	b.applyMove(m2);
	BOOST_CHECK(b.getTile(0, 1)->getLetter() == L'Z');
	BOOST_CHECK(b.getTile(0, 2)->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(horizontalCrossOutsideBoard) {
	Move m1(0, 3, Move::VERTICAL, &startTiles);
	Move m2(0, 2, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK_THROW(b.applyMove(m2), Board::InvalidMove);
}

BOOST_AUTO_TEST_CASE(horizontalAddLeftAtBorder) {
	Move m1(0, 3, Move::VERTICAL, &startTiles);
	Move m2(0, 1, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	b.applyMove(m2);
	BOOST_CHECK(b.getTile(0, 1)->getLetter() == L'Z');
	BOOST_CHECK(b.getTile(0, 2)->getLetter() == L'A');
}

BOOST_AUTO_TEST_SUITE_END();

struct MoveScoringTestSetup {
	Board b;
	std::vector<Tile *> dalTiles;
	std::vector<Tile *> zaTiles;

	MoveScoringTestSetup() : b(9, 9) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (i == 3 && j == 3) {
					b.setField(i, j, new MultiplicativeWordBonusField(3));
				} else if (i == 5 && j == 3) {
					b.setField(i, j, new ColoredField(3, GREEN));
				} else if (i == 3 && j == 5) {
					b.setField(i, j, new MultiplicativeLetterBonusField(3));
				} else {
					b.setField(i, j, new PlainField());
				}
			}
		}

		dalTiles.push_back(new Tile(L'D', 2, GREEN));
		dalTiles.push_back(new Tile(L'Ą', 5, RED));
		dalTiles.push_back(new Tile(L'Ł', 3, BLUE));

		zaTiles.push_back(new Tile(L'Z', 2, YELLOW));
		zaTiles.push_back(new Tile(L'A', 1, YELLOW));
	}

	~MoveScoringTestSetup() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				delete b.getField(i, j);
			}
		}
		while (!dalTiles.empty()) {
			Tile *t = dalTiles.back();
			dalTiles.pop_back();
			delete t;
		}
		while (!zaTiles.empty()) {
			Tile *t = zaTiles.back();
			zaTiles.pop_back();
			delete t;
		}
	}
};

BOOST_FIXTURE_TEST_SUITE(firstMoveScoring, MoveScoringTestSetup);

BOOST_AUTO_TEST_CASE(plainHorizontalMove) {
	Move m(0, 0, Move::HORIZONTAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 10);
}

BOOST_AUTO_TEST_CASE(plainVerticalMove) {
	Move m(0, 0, Move::VERTICAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 10);
}

BOOST_AUTO_TEST_CASE(tripledHorizontalMoveInsideBonus) {
	Move m(3, 2, Move::HORIZONTAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 30);
}

BOOST_AUTO_TEST_CASE(tripledVerticalMoveInsideBonus) {
	Move m(2, 3, Move::VERTICAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 30);
}

BOOST_AUTO_TEST_CASE(tripledHorizontalMoveEndBonus) {
	Move m(3, 1, Move::HORIZONTAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 30);
}

BOOST_AUTO_TEST_CASE(tripledVerticalMoveEndBonus) {
	Move m(1, 3, Move::VERTICAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 30);
}

BOOST_AUTO_TEST_CASE(tripledLetterHorizontalMoveStartBonus) {
	Move m(3, 5, Move::HORIZONTAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 14);
}

BOOST_AUTO_TEST_CASE(tripledLetterVerticalMoveStartBonus) {
	Move m(3, 5, Move::VERTICAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 14);
}

BOOST_AUTO_TEST_CASE(tripledLetterHorizontalMoveInsideBonus) {
	Move m(3, 4, Move::HORIZONTAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 20);
}

BOOST_AUTO_TEST_CASE(tripledLetterVerticalMoveInsideBonus) {
	Move m(2, 5, Move::VERTICAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 20);
}

BOOST_AUTO_TEST_CASE(coloredLetterHorizontalMoveWithBonus) {
	Move m(5, 3, Move::HORIZONTAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 14);
}

BOOST_AUTO_TEST_CASE(coloredLetterVerticalMoveWithBonus) {
	Move m(5, 3, Move::VERTICAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 14);
}

BOOST_AUTO_TEST_CASE(coloredLetterHorizontalMoveWithoutBonus) {
	Move m(5, 2, Move::HORIZONTAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 10);
}

BOOST_AUTO_TEST_CASE(coloredLetterVerticalMoveWithoutBonus) {
	Move m(4, 3, Move::VERTICAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 10);
}

BOOST_AUTO_TEST_CASE(combinedBonusHorizontal) {
	Move m(3, 3, Move::HORIZONTAL, &dalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 48);
}

BOOST_AUTO_TEST_CASE(combinedBonusVertical) {
	std::vector<Tile *> revDalTiles;
	revDalTiles.reserve(dalTiles.size());
	for (std::vector<Tile *>::reverse_iterator rit = dalTiles.rbegin(); rit != dalTiles.rend(); rit++) {
		revDalTiles.push_back(*rit);
	}
	Move m(3, 3, Move::VERTICAL, &revDalTiles);
	BOOST_CHECK_EQUAL(b.getMoveScore(m), 42);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(nextMoveScoring, MoveScoringTestSetup);

BOOST_AUTO_TEST_CASE(plainVerticalAddTop) {
	Move m1(2, 0, Move::HORIZONTAL, &dalTiles);
	Move m2(0, 0, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 5);
}

BOOST_AUTO_TEST_CASE(plainHorizontalAddLeft) {
	Move m1(0, 2, Move::VERTICAL, &dalTiles);
	Move m2(0, 0, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 5);
}

BOOST_AUTO_TEST_CASE(plainVerticalIntersect) {
	Move m1(1, 0, Move::HORIZONTAL, &dalTiles);
	Move m2(0, 1, Move::VERTICAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 8);
}

BOOST_AUTO_TEST_CASE(plainHorizontalIntersect) {
	Move m1(0, 1, Move::VERTICAL, &dalTiles);
	Move m2(1, 0, Move::HORIZONTAL, &zaTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 8);
}

BOOST_AUTO_TEST_CASE(doubleColorBonusOrthVertical) {
	Move m1(5, 4, Move::HORIZONTAL, &zaTiles);
	Move m2(5, 3, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 23);
}

BOOST_AUTO_TEST_CASE(doubleColorBonusOrthHorizontal) {
	Move m1(6, 3, Move::VERTICAL, &zaTiles);
	Move m2(5, 3, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 23);
}

BOOST_AUTO_TEST_CASE(colorBonusParallelVertical) {
	Move m1(4, 4, Move::VERTICAL, &zaTiles);
	Move m2(5, 3, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 21);
}

BOOST_AUTO_TEST_CASE(colorBonusParallelHorizontal) {
	Move m1(4, 2, Move::HORIZONTAL, &zaTiles);
	Move m2(5, 3, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 21);
}

BOOST_AUTO_TEST_CASE(colorBonusOrthVertical) {
	Move m1(4, 2, Move::HORIZONTAL, &zaTiles);
	Move m2(5, 3, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 15);
}

BOOST_AUTO_TEST_CASE(colorBonusOrthHorizontal) {
	Move m1(4, 2, Move::VERTICAL, &zaTiles);
	Move m2(5, 3, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 15);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusVerticalStart) {
	Move m1(3, 1, Move::HORIZONTAL, &zaTiles);
	Move m2(3, 3, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 45);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusVerticalMid) {
	Move m1(3, 1, Move::HORIZONTAL, &zaTiles);
	Move m2(2, 3, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 54);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusHorizontalMid) {
	Move m1(1, 3, Move::VERTICAL, &zaTiles);
	Move m2(3, 2, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 54);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusHorizontalEnd) {
	Move m1(1, 3, Move::VERTICAL, &zaTiles);
	Move m2(3, 1, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 48);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusVerticalEnd) {
	Move m1(3, 1, Move::HORIZONTAL, &zaTiles);
	Move m2(1, 3, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 48);
}

BOOST_AUTO_TEST_CASE(wordBonusOrthHorizontal) {
	Move m1(2, 1, Move::VERTICAL, &zaTiles);
	Move m2(3, 2, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 33);
}

BOOST_AUTO_TEST_CASE(wordBonusOrthVertical) {
	Move m1(1, 2, Move::HORIZONTAL, &zaTiles);
	Move m2(2, 3, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 33);
}

BOOST_AUTO_TEST_CASE(wordBonusParallelVertical) {
	Move m1(1, 2, Move::VERTICAL, &zaTiles);
	Move m2(2, 3, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 33);
}

BOOST_AUTO_TEST_CASE(wordBonusParallelHorizontal) {
	Move m1(2, 1, Move::HORIZONTAL, &zaTiles);
	Move m2(3, 2, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 33);
}

BOOST_AUTO_TEST_CASE(doubleLetterBonusOrthVertical) {
	Move m1(3, 3, Move::HORIZONTAL, &zaTiles);
	Move m2(3, 5, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 23);
}

BOOST_AUTO_TEST_CASE(doubleLetterBonusOrthHorizontal) {
	Move m1(1, 5, Move::VERTICAL, &zaTiles);
	Move m2(3, 5, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 23);
}

BOOST_AUTO_TEST_CASE(LetterBonusParallelVertical) {
	Move m1(1, 4, Move::VERTICAL, &zaTiles);
	Move m2(2, 5, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 23);
}

BOOST_AUTO_TEST_CASE(letterBonusParallelHorizontal) {
	Move m1(2, 3, Move::HORIZONTAL, &zaTiles);
	Move m2(3, 4, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 23);
}

BOOST_AUTO_TEST_CASE(letterBonusOrthVertical) {
	Move m1(1, 4, Move::HORIZONTAL, &zaTiles);
	Move m2(2, 5, Move::VERTICAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 21);
}

BOOST_AUTO_TEST_CASE(letterBonusOrthHorizontal) {
	Move m1(2, 3, Move::VERTICAL, &zaTiles);
	Move m2(3, 4, Move::HORIZONTAL, &dalTiles);

	b.applyMove(m1);
	BOOST_CHECK_EQUAL(b.getMoveScore(m2), 21);
}

BOOST_AUTO_TEST_SUITE_END();
