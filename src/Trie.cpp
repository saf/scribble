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

void Trie::Node::insert(int index, Node *node) {
	(*this->children)[index] = node;
}

Trie::Node *Trie::Node::find(int index) {
	return (*this->children)[index];
}

bool Trie::Node::isFinal() {
	return this->final;
}

void Trie::Node::setFinal() {
	this->final = true;
}

Trie::Trie(const Alphabet &alphabet) : alphabet(&alphabet), root(alphabet.getLetterCount()) {}

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

void Trie::insert(const wchar_t *word) {
	std::wstring s(word);
	this->insert(s);
}

bool Trie::find(std::wstring &word) {
	Node *node = &this->root;
	for (std::wstring::iterator it = word.begin(); it != word.end(); it++) {
		wchar_t ch = *it;
		int index = this->alphabet->getIndex(ch);
		Node *child = node->find(index);
		if (child == NULL) {
			return false;
		} else {
			node = child;
		}
	}
	return node->isFinal();
}

bool Trie::find(const wchar_t *word) {
	std::wstring s(word);
	return this->find(s);
}
