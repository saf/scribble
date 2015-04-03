/*
 * Trie.cpp
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#include "Trie.h"

Trie::Node::Node(int childCount, Node *parent) {
	this->children = new std::vector<Node *>(childCount);
	this->final = false;
	this->parent = parent;
}

Trie::Node::~Node() {
	delete this->children;
}

Trie::Node *Trie::Node::getParent() {
	return parent;
}

const Trie::Node *Trie::Node::getParent() const {
	return parent;
}

const std::vector<Trie::Node *>& Trie::Node::getChildren() const {
	return *children;
}

void Trie::Node::insert(int index, Node *node) {
	(*this->children)[index] = node;
}

Trie::Node *Trie::Node::find(int index) {
	return (*this->children)[index];
}

const Trie::Node *Trie::Node::find(int index) const {
	return (*this->children)[index];
}

bool Trie::Node::isFinal() const {
	return this->final;
}

void Trie::Node::setFinal() {
	this->final = true;
}

Trie::Trie(const Alphabet &alphabet) : alphabet(&alphabet), root(alphabet.getLetterCount()) {
	this->lastInsertedLength = 0;
	this->lastInsertedNode = &this->root;
	for (int i = 0; i < MAX_WORD_LENGTH; i++) {
		this->lastInsertedWord[i] = L'\0';
	}
}

Trie::~Trie() {}

void Trie::insert(std::wstring &word) {
	Node *node = &this->root;
	for (std::wstring::iterator it = word.begin(); it != word.end(); it++) {
		wchar_t ch = *it;
		int index = this->alphabet->getIndex(ch);
		Node *child = node->find(index);
		if (child == NULL) {
			child = new Node(this->alphabet->getLetterCount(), node);
			node->insert(index, child);
		}
		node = child;
	}
	node->setFinal();
}

//void Trie::insert(std::wstring &word) {
//	Node *node = this->lastInsertedNode;
//	std::wstring::iterator it = word.begin();
//	int commonPrefixLength = 0;
//	int i;
//
//	while (it != word.end() && *it == this->lastInsertedWord[commonPrefixLength]) {
//		it++;
//		commonPrefixLength++;
//	}
//
//	while (this->lastInsertedLength > commonPrefixLength) {
//		this->lastInsertedNode = this->lastInsertedNode->getParent();
//		this->lastInsertedLength--;
//	}
//
//	node = this->lastInsertedNode;
//	i = commonPrefixLength + 1;
//
//	while (it != word.end()) {
//		wchar_t ch = *it;
//		int index = this->alphabet->getIndex(ch);
//		Node *child = node->find(index);
//		if (child == NULL) {
//			child = new Node(this->alphabet->getLetterCount(), node);
//			node->insert(index, child);
//		}
//		node = child;
//		this->lastInsertedWord[i++] = ch;
//	}
//	this->lastInsertedWord[i] = L'\0';
//	this->lastInsertedNode = node;
//	this->lastInsertedLength = i;
//}

void Trie::insert(const wchar_t *word) {
	std::wstring s(word);
	this->insert(s);
}

bool Trie::find(std::wstring &word) const {
	const Node *node = &this->root;
	for (std::wstring::iterator it = word.begin(); it != word.end(); it++) {
		wchar_t ch = *it;
		int index = this->alphabet->getIndex(ch);
		const Node *child = node->find(index);
		if (child == NULL) {
			return false;
		} else {
			node = child;
		}
	}
	return node->isFinal();
}

bool Trie::find(const wchar_t *word) const {
	std::wstring s(word);
	return this->find(s);
}
