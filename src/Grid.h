#pragma once

#include <assert.h>
#include <functional>
#include <vector>

#include "Direction.h"

template<typename T> class Grid;
template<typename T> class Segment;

template<typename T>
class Line {
public:
	Line(Grid<T>& grid, Direction direction, uint index, uint length)
			: grid_(grid),
			  direction_(direction),
			  index_(index),
			  length_(length) {
	}

	T& operator[](uint indexInLine) {
		return grid_.atCoords(coordinates(indexInLine));
	}

	Direction direction() const {
		return direction_;
	}

	uint index() const {
		return index_;
	}

	uint length() const {
		return length_;
	}

	Line orthogonal(uint indexInLine) {
		return grid_.getLine(direction_.other(), indexInLine);
	}

	Coordinates coordinates(uint indexInLine) const {
		assert(indexInLine < length_);
		if (direction_ == Direction::HORIZONTAL) {
			return Coordinates(index_, indexInLine);
		} else {
			return Coordinates(indexInLine, index_);
		}
	}

	Segment<T> suffix(uint offset) {
		return Segment<T>(*this, offset, length() - offset);
	}

	const Segment<T> suffix(uint offset) const {
		return const_cast<Line<T>*>(this)->suffix(offset);
	}

	Segment<T> segment(uint offset, uint length) {
		return Segment<T>(*this, offset, length);
	}

	const Segment<T> segment(uint offset, uint length) const {
		return Segment<T>(*this, offset, length);
	}

private:
	Grid<T>& grid_;
	Direction direction_;
	uint index_;
	uint length_;
};

template<typename T>
class Segment {
public:
	Segment(Line<T>& line, uint offset, uint length)
			: line_(std::move(line)),
			  offset_(offset),
			  length_(length) {
		assert(offset + length <= line.length());
	}

	T& operator[](uint indexInSegment) {
		assert(indexInSegment < length_);
		return line_[offset_ + indexInSegment];
	}

	const T& operator[](uint indexInSegment) const {
		return const_cast<const T&>((*const_cast<Segment<T>*>(this))[indexInSegment]);
	}

	Direction direction() const {
		return line_.direction();
	}

	uint length() const {
		return length_;
	}

	Coordinates coordinates(uint indexInSegment) const {
		assert(indexInSegment < length_);
		return line_.coordinates(offset_ + indexInSegment);
	}

	Segment<T> suffix(uint offset) {
		return Segment<T>(line_, offset_ + offset, length() - offset_ - offset);
	}

	const Segment<T> suffix(uint offset) const {
		return const_cast<Segment<T>*>(this)->suffix(offset);
	}

private:
	Line<T> line_;
	uint offset_;
	uint length_;
};

template<typename T>
class Grid : public std::vector<std::vector<T>> {
public:
	Grid(uint rows, uint cols, std::function<T(uint, uint)> initializer) :
			rowCount_(rows),
			columnCount_(cols) {
		this->resize(rowCount_);
		for (uint row = 0; row < rows; row++) {
			for (uint col = 0; col < cols; col++) {
				Base::at(row).emplace_back(initializer(row, col));
			}
		}
	}

	Grid(const Grid<T>& other)
			: Base(other),
			  rowCount_(other.rowCount_),
			  columnCount_(other.columnCount_) {
	}

	Grid(Grid<T>&& other) {
		*this = std::move(other);
	}

	Grid<T>& operator=(const Grid<T>& other) {
		rowCount_ = other.rowCount_;
		columnCount_ = other.columnCount_;
		this->Base::operator=(other);
		return *this;
	}

	Grid<T>& operator=(Grid<T>&& other) {
		rowCount_ = other.rowCount_;
		columnCount_ = other.columnCount_;
		this->Base::operator=(std::move(other));
		return *this;
	}

	uint getRowCount() const {
		return rowCount_;
	}

	uint getColumnCount() const {
		return columnCount_;
	}

	T& atCoords(Coordinates coordinates) {
		return Base::at(coordinates.row).at(coordinates.column);
	}

	const T& atCoords(Coordinates coordinates) const {
		return Base::at(coordinates.row).at(coordinates.column);
	}

	Line<T> line(Direction direction, uint index) {
		if (direction == Direction::HORIZONTAL) {
			assert(index < rowCount_);
			return Line<T>(*this, direction, index, columnCount_);
		} else {
			assert(index < columnCount_);
			return Line<T>(*this, direction, index, rowCount_);
		}
	}

	const Line<T> line(Direction direction, uint index) const {
		return const_cast<Grid<T>*>(this)->line(direction, index);
	}

	Line<T> lineAt(Coordinates coordinates, Direction direction) {
		if (direction == Direction::HORIZONTAL) {
			return line(direction, coordinates.row);
		} else {
			return line(direction, coordinates.column);
		}
	}

	const Line<T> lineAt(Coordinates coordinates, Direction direction) const {
		return const_cast<Grid<T>*>(this)->lineAt(coordinates, direction);
	}

	Segment<T> segmentFrom(Coordinates coordinates, Direction direction) {
		return lineAt(coordinates, direction).suffix(coordinates.offsetOnLine(direction));
	}

	const Segment<T> segmentFrom(Coordinates coordinates, Direction direction) const {
		return const_cast<Grid<T>*>(this)->segmentFrom(coordinates, direction);
	}

	void foreach(std::function<void(Coordinates, T&)> function) {
		for (uint row = 0; row < getRowCount(); row++) {
			for (uint col = 0; col < getColumnCount(); col++) {
				Coordinates coords(row, col);
				function(coords, atCoords(coords));
			}
		}
	}

protected:
	using Base = std::vector<std::vector<T>>;
	uint rowCount_;
	uint columnCount_;
};

