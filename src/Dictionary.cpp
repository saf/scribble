/*
 * Dictionary.cpp
 *
 *  Created on: 08-01-2015
 *      Author: saf
 */

#include <fstream>
#include <utility>

#include "Dictionary.h"

Dictionary::Dictionary(Alphabet alphabet)
		: entryCount_(0),
		  tree_(std::move(alphabet)) {
}

void Dictionary::readFromStream(std::wistream &s) {
	while (!s.eof()) {
		Letter word[Trie::MAX_WORD_LENGTH];
		s.getline(word, Trie::MAX_WORD_LENGTH);

		tree_.insert(Word(&word[0]));
		entryCount_++;
	}
}

void Dictionary::readFromFile(std::string &filename) {
	std::wifstream ifs;
	ifs.open(filename.c_str(), std::wifstream::in);
	this->readFromStream(ifs);
}

int Dictionary::getSize() {
	return this->entryCount_;
}

bool Dictionary::check(const Word& word) {
	return tree_.find(word);
}
