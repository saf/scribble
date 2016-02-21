/*
 * Dictionary.h
 *
 *  Created on: 08-01-2015
 *      Author: saf
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <iostream>
#include <memory>

#include "Alphabet.h"
#include "Trie.h"

class Dictionary {
public:
	Dictionary(Alphabet alphabet);

	virtual ~Dictionary() {}

	virtual void readFromStream(std::wistream &stream);
	virtual void readFromFile(std::string &filename);

	virtual bool check(const Word& word);

	virtual int getSize();

protected:
	Dictionary(std::unique_ptr<Trie> tree);

	int entryCount_;
	std::unique_ptr<Trie> tree_;
};

#endif /* DICTIONARY_H_ */
