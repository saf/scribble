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
		  tree_(new Trie(std::move(alphabet))) {
}

Dictionary::Dictionary(std::unique_ptr<Trie> tree)
		: entryCount_(0),
		  tree_(std::move(tree)) {
}

void Dictionary::readFromStream(std::wistream &s) {
	while (!s.eof()) {
		Word word;
		std::getline(s, word);
		tree_->insert(word);
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
	return tree_->find(word);
}
