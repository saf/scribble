/*
 * Literaki.cpp
 *
 *  Created on: 01-02-2015
 *      Author: saf
 */

#include <assert.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>

#include "Decision.h"
#include "Fields.h"
#include "GameState.h"
#include "IdlePlayer.h"
#include "LiterakiGame.h"
#include "LiterakiBoardPrinter.h"

LiterakiGame::LiterakiGame(std::vector<std::unique_ptr<Player>> players)
	: IsoTileGame(std::move(players)) {}

LiterakiGame::LiterakiGame(LiterakiGame&& other) : IsoTileGame(std::move(other)) {
}

LiterakiGame& LiterakiGame::operator=(LiterakiGame&& other) {
	std::swap(players_, other.players_);
	std::swap(currentState_, other.currentState_);
	std::swap(stateHistory_, other.stateHistory_);
	std::swap(decisionHistory_, other.decisionHistory_);
	return *this;
}

const int LiterakiGame::RACK_SIZE = 7;

std::vector<IsoTileGame::TileGroup> LiterakiGame::getTileGroups() {
	return std::vector<TileGroup>{
		{ 9, L'a', 1, LiterakiGame::YELLOW },
		{ 1, L'ą', 5, LiterakiGame::RED },
		{ 2, L'b', 3, LiterakiGame::BLUE },
		{ 3, L'c', 2, LiterakiGame::GREEN },
		{ 1, L'ć', 5, LiterakiGame::RED },
		{ 3, L'd', 2, LiterakiGame::GREEN },
		{ 7, L'e', 1, LiterakiGame::YELLOW },
		{ 1, L'ę', 5, LiterakiGame::RED },
		{ 1, L'f', 5, LiterakiGame::RED },
		{ 2, L'g', 3, LiterakiGame::BLUE },
		{ 2, L'h', 3, LiterakiGame::BLUE },
		{ 8, L'i', 1, LiterakiGame::YELLOW },
		{ 2, L'j', 3, LiterakiGame::BLUE },
		{ 3, L'k', 2, LiterakiGame::GREEN },
		{ 3, L'l', 2, LiterakiGame::GREEN },
		{ 2, L'ł', 3, LiterakiGame::BLUE },
		{ 3, L'm', 2, LiterakiGame::GREEN },
		{ 5, L'n', 1, LiterakiGame::YELLOW },
		{ 1, L'ń', 5, LiterakiGame::RED },
		{ 6, L'o', 1, LiterakiGame::YELLOW },
		{ 1, L'ó', 5, LiterakiGame::RED },
		{ 3, L'p', 2, LiterakiGame::GREEN },
		{ 4, L'r', 1, LiterakiGame::YELLOW },
		{ 4, L's', 1, LiterakiGame::YELLOW },
		{ 1, L'ś', 5, LiterakiGame::RED },
		{ 3, L't', 2, LiterakiGame::GREEN },
		{ 2, L'u', 3, LiterakiGame::BLUE },
		{ 4, L'w', 1, LiterakiGame::YELLOW },
		{ 4, L'y', 2, LiterakiGame::GREEN },
		{ 5, L'z', 1, LiterakiGame::YELLOW },
		{ 1, L'ź', 5, LiterakiGame::RED },
		{ 1, L'ż', 5, LiterakiGame::RED },
		{ 2, L'_' },
	};
}

std::unique_ptr<Board> LiterakiGame::getInitialBoard() {
	return std::unique_ptr<Board>(new Board(15, 15, [](uint r, uint c) {
		/* The special fields are organized in city-metric
		 * circles around the center, so we'll
		 * build the construction of the board upon that. */
		int distanceFromCenter = std::abs(static_cast<int>(r) - 7) + std::abs(static_cast<int>(c) - 7);
		if (distanceFromCenter == 0) {
			return std::unique_ptr<Field>(new ColoredField(3, RED));
		} else if (distanceFromCenter == 2) {
			return std::unique_ptr<Field>(new ColoredField(3, BLUE));
		} else if (distanceFromCenter == 5) {
			return std::unique_ptr<Field>(new ColoredField(3, YELLOW));
		} else if (distanceFromCenter == 9) {
			return std::unique_ptr<Field>(new ColoredField(3, GREEN));
		} else if (distanceFromCenter == 14) {
			return std::unique_ptr<Field>(new ColoredField(3, RED));
		} else if (distanceFromCenter == 7) {
			if (std::abs(static_cast<int>(r) - 7) <= 1 || std::abs(static_cast<int>(c) - 7) <= 1) {
				return std::unique_ptr<Field>(new ColoredField(3, RED));
			} else {
				return std::unique_ptr<Field>(new MultiplicativeWordBonusField(2));
			}
		} else if (distanceFromCenter == 12 && r != 1 && r != 13) {
			return std::unique_ptr<Field>(new MultiplicativeWordBonusField(3));
		} else {
			return std::unique_ptr<Field>(new PlainField());
		}
	}));
}

int LiterakiGame::getRackSize() const {
	return RACK_SIZE;
}

std::pair<Tiles, BlankAssignments> LiterakiGame::getMoveTiles(
		const Segment<std::shared_ptr<Tile>>& segment, const Rack& playerRack, Word moveString) {
	Tiles moveTiles;
	BlankAssignments blankAssignments;
	Rack rack = playerRack;

	bool blankTile = false;
	uint index = 0;

	for (Letter letter : moveString) {
		if (letter == LETTER('[')) {
			blankTile = true;
		} else if (letter == LETTER(']')) {
			blankTile = false;
		} else {
			if (segment[index] == nullptr) {
				std::shared_ptr<Tile> tile;
				if (blankTile) {
					tile = IsoTileGame::extractTile(rack, LETTER('_'));
					if (tile != nullptr) {
						blankAssignments.add(tile, letter);
					}
				} else {
					tile = IsoTileGame::extractTile(rack, letter);
				}
				if (tile != nullptr) {
					moveTiles.push_back(tile);
				} else {
					throw "Failed to find tile for player move.";
				}
			}
			++index;
		}
	}
	return std::make_pair(moveTiles, blankAssignments);
}

Tiles LiterakiGame::getMissingRackTiles(const Bag& gameBag, const Rack& playerRack, Word rackLetters) {
	Bag bag = gameBag;
	Rack rack = playerRack;
	Tiles tiles;

	for (Letter letter : rackLetters) {
		auto tile = IsoTileGame::extractTile(rack, letter);
		if (tile == nullptr) {
			tile = IsoTileGame::extractTile(bag, letter);
			if (tile != nullptr) {
				tiles.push_back(tile);
			} else {
				throw "Failed to find missing tile for player rack";
			}
		}
	}
	return tiles;
}

Tiles LiterakiGame::getRackTiles(const Rack& playerRack, Word letters) {
	Rack rack = playerRack;
	Tiles tiles;

	for (Letter letter : letters) {
		auto tile = IsoTileGame::extractTile(rack, letter);
		if (tile != nullptr) {
			tiles.push_back(tile);
		} else {
			throw "Failed to find tile in player rack";
		}
	}
	return tiles;
}

LiterakiGame LiterakiGame::readFromStream(std::wistream &stream) {
	Players players;
	players.emplace_back(new IdlePlayer());
	players.emplace_back(new IdlePlayer());
	LiterakiGame game(std::move(players));
	game.initializeState();

	int turn = 0;
	int round = 1;

	while (!stream.eof()) {
		std::wstring line;
		std::getline(stream, line);
		const wchar_t *str = line.c_str();

		if (strchr("#\r\n", str[0])) {
			/* Skip comments & blank lines */
			continue;
		}

		std::wistringstream lineStream(line);

		if (turn == 0) {
			int readRound;
			wchar_t dot;
			lineStream >> readRound >> dot;
			assert(readRound == round);
		}

		std::wstring rackString;
		lineStream >> rackString;

		assert(lineStream.good());
		/* There is a colon in the 'rack' string, so we need to get rid of that. */
		rackString.pop_back();

		const Bag& bag = game.getCurrentState().getBag();
		const Rack& rack = game.getCurrentState().getRacks()[turn];

		Tiles missingRackTiles = getMissingRackTiles(bag, rack, rackString);
		assert(missingRackTiles.size() + rack.size() == rackString.length());
		if (game.stateHistory_.size() > 0) {
			game.stateHistory_.back()->repopulateRack(turn, missingRackTiles);
		}
		game.currentState_->repopulateRack(turn, missingRackTiles);

		if (line.find(L"L") != std::string::npos) {
			/* End-of-game */
			game.applyDecision(std::shared_ptr<Decision>(new PassDecision()));
		} else if (line.find(L"+") != std::string::npos) {
			wchar_t column;
			int row;
			wchar_t direction;
			wchar_t plus;
			std::wstring words;
			int points;

			lineStream >> column >> row >> direction >> words >> plus >> points;
			int col = column - 'a';
			row--;    // Row is 1-based in game logs.

			size_t firstSlash = words.find_first_of(L'/');
			std::wstring mainWord = firstSlash != std::wstring::npos ? words.substr(0, firstSlash) : words;

			Direction dir = direction == L'+' ? Direction::VERTICAL : Direction::HORIZONTAL;
			const Segment<std::shared_ptr<Tile>> segment =
					game.getCurrentState().getTiles().segmentFrom(Coordinates(row, col), dir);

			Tiles moveTiles;
			BlankAssignments blankAssignments;
			std::tie(moveTiles, blankAssignments) = getMoveTiles(segment, rack, mainWord);

			std::shared_ptr<MoveDecision> decision = std::shared_ptr<MoveDecision>(
					new MoveDecision(Move(Coordinates(row, col), dir, moveTiles, blankAssignments))
			);

			int computedPoints = game.score(game.currentState_->getTiles(), decision->getMove());
			assert(points == computedPoints);
			game.applyDecision(decision);
		} else if (line.find(L"->") != std::string::npos) {
			std::wstring removedLetters;
			lineStream >> removedLetters;
			assert(lineStream.good());
			Tiles removedTiles = getRackTiles(rack, removedLetters);
			std::shared_ptr<TileExchangeDecision> decision = std::shared_ptr<TileExchangeDecision>(
					new TileExchangeDecision(removedTiles)
			);
			game.applyDecision(decision);
		} else {
			game.applyDecision(std::shared_ptr<Decision>(new PassDecision()));
		}

		if (turn == 1) {
			round++;
		}
		turn = (turn + 1) % 2;
	}

	return game;
}

static int wordScore(const Board& board, const Segment<std::shared_ptr<Tile>>& segment, const Tiles& moveTiles) {
	uint index = 0;
	Tiles::const_iterator nextTile = moveTiles.begin();
	size_t count = moveTiles.size();
	int score = 0;
	std::vector<std::function<void(int&)>> wordScoreModifiers;
	while (index < segment.length() && (segment[index] != nullptr || nextTile != moveTiles.end())) {
		const Field& field = board.getField(segment.coordinates(index));
		if (segment[index] == nullptr) {
			const std::shared_ptr<Tile>& tilePtr = *nextTile;
			field.applyScore(*tilePtr, true, score);
			wordScoreModifiers.emplace_back([&field](int& score) { field.changeWordScore(true, score); });
			nextTile++;
		} else {
			field.applyScore(*segment[index], false, score);
			wordScoreModifiers.emplace_back([&field](int& score) { field.changeWordScore(false, score); });
		}
		index++;
	}

	for (auto& wordModifier : wordScoreModifiers) {
		wordModifier(score);
	}

	return score;
}

int LiterakiGame::score(const TilePlacement& tiles, const Move& move) const {
	Coordinates startCoords(move.getCoordinates());
	Direction direction = move.getDirection();

	const Tiles& moveTiles = move.getTiles();
	int score = 0;
	auto line = tiles.lineAt(startCoords, direction);

	// Create a segment which starts at the first tile of the created/expanded word.
	uint startOffset = static_cast<int>(startCoords.offsetOnLine(direction));
	int previous = static_cast<int>(startOffset) - 1;
	while (previous >= 0 && line[previous] != nullptr) {
		startOffset = previous;
		previous--;
	}

	score += wordScore(tiles.getBoard(),
			Segment<std::shared_ptr<Tile>>(line, startOffset, line.length() - startOffset),
			moveTiles
	);

	uint index = startOffset;
	for (const auto& tile : moveTiles) {
		while (line[index] != nullptr) {
			index++;
		}
		auto orthogonalSegment = tiles.filledSegmentAround(line.coordinates(index), direction.other());
		if (orthogonalSegment.length() > 1) {
			score += wordScore(tiles.getBoard(), orthogonalSegment, Tiles{tile});
		}
		index++;
	}

	// Bonus for placing 7 tiles.
	if (moveTiles.size() >= 7) {
		score += 50;
	}

	return score;
}
