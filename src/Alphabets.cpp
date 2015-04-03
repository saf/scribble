#include "Alphabets.h"
#include <iostream>

Alphabet::Alphabet(std::wstring diacritics) {
	this->diacritics = new std::wstring(diacritics);
}

Alphabet::~Alphabet() {
	delete this->diacritics;
}

int Alphabet::getLetterCount() const {
	return L'Z' - L'A' + 1 + this->diacritics->length();
}

int Alphabet::getIndex(unsigned wchar_t letter) const {
	if (letter >= L'A' && letter <= L'Z') {
		return static_cast<unsigned int>(letter) - static_cast<unsigned int>(L'A');
	} else {
		int base = L'Z' - L'A' + 1;
		for (int i = 0; i < this->diacritics->length(); i++) {
			if ((*this->diacritics)[i] == letter) {
				return base + i;
			}
		}
	}

	throw std::string("Failed to find index for code point.");
}

wchar_t Alphabet::getLetter(int index) const {
	if (index <= L'Z' - L'A') {
		return L'A' + index;
	} else {
		return this->diacritics->at(L'Z' - L'A' + 1 + index);
	}
}

PolishAlphabet::PolishAlphabet() : Alphabet(std::wstring(L"ĄĆĘŁŃÓŚŹŻ#")) {};

EnglishAlphabet::EnglishAlphabet() : Alphabet(std::wstring(L"#")) {};

