#ifndef _ALPHABETS_H_
#define _ALPHABETS_H_

#include <string>

using Letter = wchar_t;
#define _CAT(a, b) a##b
#define LETTER(ch) _CAT(L, ch)

using Word = std::wstring;
#define WORD(w) _CAT(L, w)

/**
 * Defines a set of letters and mapping of letters to indices
 * in lookup tables for a Trie.
 */
class Alphabet {
public:
	Alphabet(Word diacritics);
	virtual ~Alphabet() {}

	virtual unsigned int getIndex(Letter letter) const;
	virtual Letter getLetter(unsigned int index) const;
	unsigned int getLetterCount() const;
private:
	Word diacritics_;
};

#endif
