#ifndef _BLANK_ASSIGNMENT_H_
#define _BLANK_ASSIGNMENT_H_

#include <memory>

#include "Alphabet.h"
#include "Basics.h"
#include "Tile.h"

struct BlankAssignment {
	BlankAssignment(std::shared_ptr<Tile> tile, Letter letter)
			: tile(std::move(tile)),
			  letter(letter) {
	}

	std::shared_ptr<Tile> tile;

	Letter letter;
};

class BlankAssignments {
public:
	void add(std::shared_ptr<Tile> tile, Letter letter) {
		assignments_.emplace_back(tile, letter);
	}

	void add(BlankAssignments newAssignments) {
		std::move(newAssignments.assignments_.begin(),
				newAssignments.assignments_.end(),
				std::back_inserter(assignments_));
	}

	Letter find(const std::shared_ptr<Tile>& tile) {
		for (const BlankAssignment& assignment : assignments_) {
			if (assignment.tile == tile) {
				return assignment.letter;
			}
		}
		return L'_';
	}
private:
	std::vector<BlankAssignment> assignments_;
};

#endif
