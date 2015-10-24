#include "Direction.h"

Direction::Direction(Directions dir)
		: direction_(dir) {
}


bool Direction::operator==(const Direction& other) const {
	return direction_ == other.direction_;
}

Direction Direction::other() const {
	if (direction_ == Directions::HORIZONTAL) {
		return VERTICAL;
	} else {
		return HORIZONTAL;
	}
}

const Direction Direction::HORIZONTAL = Direction(Direction::Directions::HORIZONTAL);
const Direction Direction::VERTICAL = Direction(Direction::Directions::VERTICAL);
