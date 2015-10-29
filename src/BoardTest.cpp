#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Board

#include <boost/test/unit_test.hpp>

#include <cstdio>
#include <memory>

#include "Basics.h"
#include "Board.h"
#include "Fields.h"
#include "LiterakiGame.h"
#include "TilePlacement.h"

enum colors { PLAIN, YELLOW, GREEN, BLUE, RED };

struct MoveManagementTestSetup {
	Board b;
	TilePlacement t;
	Tiles startTiles;
	Tiles zaTiles;

	MoveManagementTestSetup()
			: b(5, 4, [](uint, uint) { return std::unique_ptr<Field>(new PlainField()); }),
			  t(b) {
		startTiles.push_back(std::make_shared<Tile>(L'D', 2, GREEN));
		startTiles.push_back(std::make_shared<Tile>(L'Ą', 5, RED));
		startTiles.push_back(std::make_shared<Tile>(L'Ł', 3, BLUE));

		zaTiles.push_back(std::make_shared<Tile>(L'Z', 2, YELLOW));
		zaTiles.push_back(std::make_shared<Tile>(L'A', 1, YELLOW));
	}
};

BOOST_FIXTURE_TEST_SUITE(checkFirstMove, MoveManagementTestSetup);

BOOST_AUTO_TEST_CASE(checkMoveLeftOfBoard) {
	Move m(Coordinates(0, -1), Direction::HORIZONTAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == false);
}

BOOST_AUTO_TEST_CASE(checkMoveAboveBoard) {
	Move m(Coordinates(-1, 0), Direction::HORIZONTAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == false);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBoardTopBorder) {
	Move m(Coordinates(0, 1), Direction::HORIZONTAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBoardLeftBorder) {
	Move m(Coordinates(1, 0), Direction::VERTICAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBoardLastRow) {
	Move m(Coordinates(3, 0), Direction::HORIZONTAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBoardLastColumn) {
	Move m(Coordinates(0, 3), Direction::VERTICAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtRightBorder) {
	Move m(Coordinates(0, 1), Direction::HORIZONTAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveAtBottomBorder) {
	Move m(Coordinates(2, 0), Direction::VERTICAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == true);
}

BOOST_AUTO_TEST_CASE(checkMoveOverRightBorder) {
	Move m(Coordinates(0, 2), Direction::HORIZONTAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == false);
}

BOOST_AUTO_TEST_CASE(checkMoveOverBottomBorder) {
	Move m(Coordinates(3, 0), Direction::VERTICAL, startTiles);
	BOOST_CHECK(t.checkMove(m) == false);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(firstMovePlacement, MoveManagementTestSetup);

BOOST_AUTO_TEST_CASE(horizontalWordAtTopLeftCorner) {
	Move m(Coordinates(0, 0), Direction::HORIZONTAL, startTiles);
	t.applyMove(m);
	for (size_t i = 0; i < startTiles.size(); i++) {
		BOOST_CHECK_EQUAL(t[0][i]->getLetter(), startTiles[i]->getLetter());
	}
}

BOOST_AUTO_TEST_CASE(verticalWordAtTopLeftCorner) {
	Move m(Coordinates(0, 0), Direction::VERTICAL, startTiles);
	t.applyMove(m);
	for (size_t i = 0; i < startTiles.size(); i++) {
		BOOST_CHECK_EQUAL(t[i][0]->getLetter(), startTiles[i]->getLetter());
	}
}

BOOST_AUTO_TEST_CASE(horizontalWordAtBottomRightCorner) {
	Move m(Coordinates(4, 1), Direction::HORIZONTAL, startTiles);
	t.applyMove(m);
	for (size_t i = 0; i < startTiles.size(); i++) {
		BOOST_CHECK_EQUAL(t[4][1 + i]->getLetter(), startTiles[i]->getLetter());
	}
}

BOOST_AUTO_TEST_CASE(verticalWordAtBottomRightCorner) {
	Move m(Coordinates(2, 3), Direction::VERTICAL, startTiles);
	t.applyMove(m);
	for (size_t i = 0; i < startTiles.size(); i++) {
		BOOST_CHECK_EQUAL(t[2 + i][3]->getLetter(), startTiles[i]->getLetter());
	}
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(checkSecondMove, MoveManagementTestSetup);

BOOST_AUTO_TEST_CASE(checkVerticalCrossInsideBoard) {
	Move m1(Coordinates(1, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(0, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(t.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkVerticalAddTopInsideBoard) {
	Move m1(Coordinates(2, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(0, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(t.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkVerticalAddBottomInsideBoard) {
	Move m1(Coordinates(0, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(1, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(t.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkVerticalCrossOutsideBoard) {
	Move m1(Coordinates(4, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(3, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(!t.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkVerticalAddTopAtBorder) {
	Move m1(Coordinates(4, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(2, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(t.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalCrossInsideBoard) {
	Move m1(Coordinates(0, 1), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 0), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(t.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalAddLeftInsideBoard) {
	Move m1(Coordinates(0, 2), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 0), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(t.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalAddRightInsideBoard) {
	Move m1(Coordinates(0, 0), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 1), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(t.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalCrossOutsideBoard) {
	Move m1(Coordinates(0, 3), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 2), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(!t.checkMove(m2));
}

BOOST_AUTO_TEST_CASE(checkHorizontalAddLeftAtBorder) {
	Move m1(Coordinates(0, 3), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 1), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK(t.checkMove(m2));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(secondMovePlacement, MoveManagementTestSetup);

BOOST_AUTO_TEST_CASE(verticalCrossInsideBoard) {
	Move m1(Coordinates(1, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(0, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	t.applyMove(m2);
	BOOST_CHECK(t[0][0]->getLetter() == L'Z');
	BOOST_CHECK(t[2][0]->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(verticalAddTopInsideBoard) {
	Move m1(Coordinates(2, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(0, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	t.applyMove(m2);
	BOOST_CHECK(t[0][0]->getLetter() == L'Z');
	BOOST_CHECK(t[1][0]->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(verticalAddBottomInsideBoard) {
	Move m1(Coordinates(0, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(1, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	t.applyMove(m2);
	BOOST_CHECK(t[1][0]->getLetter() == L'Z');
	BOOST_CHECK(t[2][0]->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(verticalCrossOutsideBoard) {
	Move m1(Coordinates(4, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(3, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_THROW(t.applyMove(m2), InvalidMove);
}

BOOST_AUTO_TEST_CASE(verticalAddTopAtBorder) {
	Move m1(Coordinates(4, 0), Direction::HORIZONTAL, startTiles);
	Move m2(Coordinates(2, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	t.applyMove(m2);
	BOOST_CHECK(t[2][0]->getLetter() == L'Z');
	BOOST_CHECK(t[3][0]->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(horizontalCrossInsideBoard) {
	Move m1(Coordinates(0, 1), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 0), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	t.applyMove(m2);
	BOOST_CHECK(t[0][0]->getLetter() == L'Z');
	BOOST_CHECK(t[0][2]->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(horizontalAddLeftInsideBoard) {
	Move m1(Coordinates(0, 2), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 0), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	t.applyMove(m2);
	BOOST_CHECK(t[0][0]->getLetter() == L'Z');
	BOOST_CHECK(t[0][1]->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(horizontalAddRightInsideBoard) {
	Move m1(Coordinates(0, 0), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 1), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	t.applyMove(m2);
	BOOST_CHECK(t[0][1]->getLetter() == L'Z');
	BOOST_CHECK(t[0][2]->getLetter() == L'A');
}

BOOST_AUTO_TEST_CASE(horizontalCrossOutsideBoard) {
	Move m1(Coordinates(0, 3), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 2), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_THROW(t.applyMove(m2), InvalidMove);
}

BOOST_AUTO_TEST_CASE(horizontalAddLeftAtBorder) {
	Move m1(Coordinates(0, 3), Direction::VERTICAL, startTiles);
	Move m2(Coordinates(0, 1), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	t.applyMove(m2);
	BOOST_CHECK(t[0][1]->getLetter() == L'Z');
	BOOST_CHECK(t[0][2]->getLetter() == L'A');
}

BOOST_AUTO_TEST_SUITE_END();

struct MoveScoringTestSetup {
	Board b;
	TilePlacement t;
	LiterakiGame g;
	Tiles dalTiles;
	Tiles zaTiles;

	MoveScoringTestSetup()
			: b(9, 9, &MoveScoringTestSetup::fieldInitializer),
			  t(b),
			  g({}) {
		dalTiles.push_back(std::make_shared<Tile>(L'D', 2, GREEN));
		dalTiles.push_back(std::make_shared<Tile>(L'Ą', 5, RED));
		dalTiles.push_back(std::make_shared<Tile>(L'Ł', 3, BLUE));

		zaTiles.push_back(std::make_shared<Tile>(L'Z', 2, YELLOW));
		zaTiles.push_back(std::make_shared<Tile>(L'A', 1, YELLOW));
	}
private:
	static std::unique_ptr<Field> fieldInitializer(uint i, uint j) {
		if (i == 3 && j == 3) {
			return std::unique_ptr<Field>(new MultiplicativeWordBonusField(3));
		} else if (i == 5 && j == 3) {
			return std::unique_ptr<Field>(new ColoredField(3, GREEN));
		} else if (i == 3 && j == 5) {
			return std::unique_ptr<Field>(new MultiplicativeLetterBonusField(3));
		} else {
			return std::unique_ptr<Field>(new PlainField());
		}
	};
};

BOOST_FIXTURE_TEST_SUITE(firstMoveScoring, MoveScoringTestSetup);

BOOST_AUTO_TEST_CASE(plainHorizontalMove) {
	Move m(Coordinates(0, 0), Direction::HORIZONTAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 10);
}

BOOST_AUTO_TEST_CASE(plainVerticalMove) {
	Move m(Coordinates(0, 0), Direction::VERTICAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 10);
}

BOOST_AUTO_TEST_CASE(tripledHorizontalMoveInsideBonus) {
	Move m(Coordinates(3, 2), Direction::HORIZONTAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 30);
}

BOOST_AUTO_TEST_CASE(tripledVerticalMoveInsideBonus) {
	Move m(Coordinates(2, 3), Direction::VERTICAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 30);
}

BOOST_AUTO_TEST_CASE(tripledHorizontalMoveEndBonus) {
	Move m(Coordinates(3, 1), Direction::HORIZONTAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 30);
}

BOOST_AUTO_TEST_CASE(tripledVerticalMoveEndBonus) {
	Move m(Coordinates(1, 3), Direction::VERTICAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 30);
}

BOOST_AUTO_TEST_CASE(tripledLetterHorizontalMoveStartBonus) {
	Move m(Coordinates(3, 5), Direction::HORIZONTAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 14);
}

BOOST_AUTO_TEST_CASE(tripledLetterVerticalMoveStartBonus) {
	Move m(Coordinates(3, 5), Direction::VERTICAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 14);
}

BOOST_AUTO_TEST_CASE(tripledLetterHorizontalMoveInsideBonus) {
	Move m(Coordinates(3, 4), Direction::HORIZONTAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 20);
}

BOOST_AUTO_TEST_CASE(tripledLetterVerticalMoveInsideBonus) {
	Move m(Coordinates(2, 5), Direction::VERTICAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 20);
}

BOOST_AUTO_TEST_CASE(coloredLetterHorizontalMoveWithBonus) {
	Move m(Coordinates(5, 3), Direction::HORIZONTAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 14);
}

BOOST_AUTO_TEST_CASE(coloredLetterVerticalMoveWithBonus) {
	Move m(Coordinates(5, 3), Direction::VERTICAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 14);
}

BOOST_AUTO_TEST_CASE(coloredLetterHorizontalMoveWithoutBonus) {
	Move m(Coordinates(5, 2), Direction::HORIZONTAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 10);
}

BOOST_AUTO_TEST_CASE(coloredLetterVerticalMoveWithoutBonus) {
	Move m(Coordinates(4, 3), Direction::VERTICAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 10);
}

BOOST_AUTO_TEST_CASE(combinedBonusHorizontal) {
	Move m(Coordinates(3, 3), Direction::HORIZONTAL, dalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 48);
}

BOOST_AUTO_TEST_CASE(combinedBonusVertical) {
	Tiles revDalTiles;
	revDalTiles.reserve(dalTiles.size());
	for (Tiles::reverse_iterator rit = dalTiles.rbegin(); rit != dalTiles.rend(); rit++) {
		revDalTiles.push_back(*rit);
	}
	Move m(Coordinates(3, 3), Direction::VERTICAL, revDalTiles);
	BOOST_CHECK_EQUAL(g.score(t, m), 42);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(nextMoveScoring, MoveScoringTestSetup);

BOOST_AUTO_TEST_CASE(plainVerticalAddTop) {
	Move m1(Coordinates(2, 0), Direction::HORIZONTAL, dalTiles);
	Move m2(Coordinates(0, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 5);
}

BOOST_AUTO_TEST_CASE(plainHorizontalAddLeft) {
	Move m1(Coordinates(0, 2), Direction::VERTICAL, dalTiles);
	Move m2(Coordinates(0, 0), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 5);
}

BOOST_AUTO_TEST_CASE(plainVerticalIntersect) {
	Move m1(Coordinates(1, 0), Direction::HORIZONTAL, dalTiles);
	Move m2(Coordinates(0, 1), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 8);
}

BOOST_AUTO_TEST_CASE(plainHorizontalIntersect) {
	Move m1(Coordinates(0, 1), Direction::VERTICAL, dalTiles);
	Move m2(Coordinates(1, 0), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 8);
}

BOOST_AUTO_TEST_CASE(plainVerticalAddBottom) {
	Move m1(Coordinates(0, 0), Direction::HORIZONTAL, dalTiles);
	Move m2(Coordinates(1, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 5);
}

BOOST_AUTO_TEST_CASE(plainVerticalAddBottom2) {
	Move m1(Coordinates(0, 0), Direction::HORIZONTAL, dalTiles);
	Move m2(Coordinates(0, 0), Direction::VERTICAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 5);
}

BOOST_AUTO_TEST_CASE(plainHorizontalAddRight) {
	Move m1(Coordinates(0, 0), Direction::VERTICAL, dalTiles);
	Move m2(Coordinates(0, 1), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 5);
}

BOOST_AUTO_TEST_CASE(plainHorizontalAddRight2) {
	Move m1(Coordinates(0, 0), Direction::VERTICAL, dalTiles);
	Move m2(Coordinates(0, 0), Direction::HORIZONTAL, zaTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 5);
}

BOOST_AUTO_TEST_CASE(doubleColorBonusOrthVertical) {
	Move m1(Coordinates(5, 4), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(5, 3), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 23);
}

BOOST_AUTO_TEST_CASE(doubleColorBonusOrthHorizontal) {
	Move m1(Coordinates(6, 3), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(5, 3), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 23);
}

BOOST_AUTO_TEST_CASE(colorBonusParallelVertical) {
	Move m1(Coordinates(4, 4), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(5, 3), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 21);
}

BOOST_AUTO_TEST_CASE(colorBonusParallelHorizontal) {
	Move m1(Coordinates(4, 2), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(5, 3), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 21);
}

BOOST_AUTO_TEST_CASE(colorBonusOrthVertical) {
	Move m1(Coordinates(4, 2), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(5, 3), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 15);
}

BOOST_AUTO_TEST_CASE(colorBonusOrthHorizontal) {
	Move m1(Coordinates(4, 2), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(5, 3), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 15);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusVerticalStart) {
	Move m1(Coordinates(3, 1), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(3, 3), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 45);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusVerticalMid) {
	Move m1(Coordinates(3, 1), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(2, 3), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 54);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusHorizontalMid) {
	Move m1(Coordinates(1, 3), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(3, 2), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 54);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusHorizontalEnd) {
	Move m1(Coordinates(1, 3), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(3, 1), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 48);
}

BOOST_AUTO_TEST_CASE(doubleWordBonusVerticalEnd) {
	Move m1(Coordinates(3, 1), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(1, 3), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 48);
}

BOOST_AUTO_TEST_CASE(wordBonusOrthHorizontal) {
	Move m1(Coordinates(2, 1), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(3, 2), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 33);
}

BOOST_AUTO_TEST_CASE(wordBonusOrthVertical) {
	Move m1(Coordinates(1, 2), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(2, 3), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 33);
}

BOOST_AUTO_TEST_CASE(wordBonusParallelVertical) {
	Move m1(Coordinates(1, 2), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(2, 3), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 33);
}

BOOST_AUTO_TEST_CASE(wordBonusParallelHorizontal) {
	Move m1(Coordinates(2, 1), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(3, 2), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 33);
}

BOOST_AUTO_TEST_CASE(doubleLetterBonusOrthVertical) {
	Move m1(Coordinates(3, 3), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(3, 5), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 23);
}

BOOST_AUTO_TEST_CASE(doubleLetterBonusOrthHorizontal) {
	Move m1(Coordinates(1, 5), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(3, 5), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 23);
}

BOOST_AUTO_TEST_CASE(LetterBonusParallelVertical) {
	Move m1(Coordinates(1, 4), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(2, 5), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 23);
}

BOOST_AUTO_TEST_CASE(letterBonusParallelHorizontal) {
	Move m1(Coordinates(2, 3), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(3, 4), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 23);
}

BOOST_AUTO_TEST_CASE(letterBonusOrthVertical) {
	Move m1(Coordinates(1, 4), Direction::HORIZONTAL, zaTiles);
	Move m2(Coordinates(2, 5), Direction::VERTICAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 21);
}

BOOST_AUTO_TEST_CASE(letterBonusOrthHorizontal) {
	Move m1(Coordinates(2, 3), Direction::VERTICAL, zaTiles);
	Move m2(Coordinates(3, 4), Direction::HORIZONTAL, dalTiles);

	t.applyMove(m1);
	BOOST_CHECK_EQUAL(g.score(t, m2), 21);
}

BOOST_AUTO_TEST_SUITE_END();
