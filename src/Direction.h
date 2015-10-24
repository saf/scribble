#pragma once

class Direction {
private:
	enum class Directions { HORIZONTAL, VERTICAL };

	Direction(Directions dir);

	Directions direction_;
public:
	static const Direction HORIZONTAL;
	static const Direction VERTICAL;

	bool operator==(const Direction& other) const;

	Direction other() const;
};

