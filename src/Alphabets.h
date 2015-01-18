#ifndef _ALPHABETS_H_
#define _ALPHABETS_H_

#include <string>

/**
 * Defines a set of letters and mapping of letters to indices
 * in lookup tables for a Trie.
 */
class Alphabet {
private:
	std::wstring *diacritics;
public:
	Alphabet(std::wstring diacritics);
	virtual ~Alphabet();

	virtual int getIndex(unsigned wchar_t letter) const;
	int getLetterCount() const;
};

class PolishAlphabet : public Alphabet {
public:
	PolishAlphabet();
	virtual ~PolishAlphabet() {};
};

class EnglishAlphabet : public Alphabet {
	EnglishAlphabet();
	virtual ~EnglishAlphabet() {};
};

#endif
