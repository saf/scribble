/*
 * Dictionary.h
 *
 *  Created on: 08-01-2015
 *      Author: saf
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <iostream>
#include "Alphabets.h"
#include "Trie.h"

class Dictionary {
protected:
	Trie *tree;
	int size;

	static const long int MAX_WORD_LENGTH = 40;
public:
	Dictionary(const Alphabet &alphabet);
	virtual ~Dictionary();

	void readFromStream(std::wistream &stream);
	void readFromFile(std::string &filename);

	bool check(std::wstring &word);

	int getSize();
};

#endif /* DICTIONARY_H_ */
