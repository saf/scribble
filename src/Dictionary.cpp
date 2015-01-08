/*
 * Dictionary.cpp
 *
 *  Created on: 08-01-2015
 *      Author: saf
 */

#include <fstream>
#include "Dictionary.h"

Dictionary::Dictionary(const Alphabet &alphabet) {
	this->size = 0;
	this->tree = new Trie(alphabet);
}

Dictionary::~Dictionary() {
	delete this->tree;
}

void Dictionary::readFromStream(std::wistream &s) {
	while (!s.eof()) {
		wchar_t word[MAX_WORD_LENGTH];
		s.getline(word, MAX_WORD_LENGTH);

		tree->insert(word);
		this->size++;
	}
}

void Dictionary::readFromFile(std::string &filename) {
	std::wifstream ifs;
	ifs.open(filename.c_str(), std::wifstream::in);
	this->readFromStream(ifs);
}

int Dictionary::getSize() {
	return this->size;
}

bool Dictionary::check(std::wstring &word) {
	return tree->find(word);
}
