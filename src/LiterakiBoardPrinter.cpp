/*
 * LiterakiBoardPrinter.cpp
 *
 *  Created on: 05-02-2015
 *      Author: saf
 */

#include "LiterakiBoardPrinter.h"
#include "LiterakiGame.h"
#include <cstdio>

LiterakiBoardPrinter::LiterakiBoardPrinter() {}

LiterakiBoardPrinter::~LiterakiBoardPrinter() {}

std::wstring LiterakiBoardPrinter::getTileString(const Tile *tile) {
	return BoardPrinter::getTileString(tile);
}

std::wstring LiterakiBoardPrinter::getFieldString(const Field *field, const Tile *tile) {
	this->tileString = getTileString(tile);
	field->accept(*this); /* This sets our 'fieldString' member */
	return this->fieldString;
}

void LiterakiBoardPrinter::visit(const PlainField *field) {
	this->fieldString = this->tileString + L" ";
}

void LiterakiBoardPrinter::visit(const ColoredField *field) {
	wchar_t s[20];
	const wchar_t *color = L"0";

	switch (field->getColor()) {
	case LiterakiGame::RED:
		color = L"1";
		break;
	case LiterakiGame::BLUE:
		color = L"4";
		break;
	case LiterakiGame::GREEN:
		color = L"2";
		break;
	case LiterakiGame::YELLOW:
		color = L"3";
		break;
	}

	swprintf(s, 20, L"\033[1;3%s;7;41m%ls \033[0m", color, this->tileString.c_str());
	this->fieldString = std::wstring(s);
}

void LiterakiBoardPrinter::visit(const MultiplicativeWordBonusField *field) {
	wchar_t s[20];
	const wchar_t *color = L"0";

	switch (field->getFactor()) {
	case 2:
		color = L"7";
		break;
	case 3:
		color = L"0";
		break;
	}

	swprintf(s, 20, L"\033[1;3%s;7;41m%ls \033[0m", color, this->tileString.c_str());
	this->fieldString = std::wstring(s);
}

void LiterakiBoardPrinter::visit(const MultiplicativeLetterBonusField *field) {
	this->fieldString = this->tileString;
}
