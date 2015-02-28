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

public:
	Dictionary(const Alphabet &alphabet);
	virtual ~Dictionary();

	virtual void readFromStream(std::wistream &stream);
	virtual void readFromFile(std::string &filename);

	virtual bool check(std::wstring &word);

	virtual int getSize();
};

#endif /* DICTIONARY_H_ */
