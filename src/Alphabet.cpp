#include "Alphabet.h"

#include <assert.h>
#include <iostream>

static const wchar_t kFirstBaseLetter = LETTER('a');
static const wchar_t kLastBaseLetter = LETTER('z');
static const unsigned int kBaseLetterCount = kLastBaseLetter - kFirstBaseLetter + 1;

Alphabet::Alphabet(std::wstring diacritics)
		: diacritics_(diacritics + LETTER('#')) {
}

unsigned int Alphabet::getLetterCount() const {
	return kLastBaseLetter - kFirstBaseLetter + 1 + diacritics_.length();
}

unsigned int Alphabet::getIndex(Letter letter) const {
	if (letter >= kFirstBaseLetter && letter <= kLastBaseLetter) {
		return static_cast<unsigned int>(letter) - static_cast<unsigned int>(LETTER('a'));
	} else {
		size_t index = diacritics_.find_first_of(letter);
		assert(index != std::wstring::npos);
		return kBaseLetterCount + index;
	}
}

wchar_t Alphabet::getLetter(unsigned int index) const {
	if (index < kBaseLetterCount) {
		return kFirstBaseLetter + index;
	} else {
		return diacritics_.at(kBaseLetterCount + index);
	}
}

