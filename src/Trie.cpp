/*
 * Trie.cpp
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#include "Trie.h"

Trie::Node::Node(const Alphabet& alphabet, const Node* parent)
		: parent(parent),
		  children(alphabet.getLetterCount()),
		  final(false) {
}

const Trie::Node* Trie::Node::getParent() const {
	return parent;
}

const std::vector<std::unique_ptr<Trie::Node>>& Trie::Node::getChildren() const {
	return children;
}

std::unique_ptr<Trie::Node>& Trie::Node::find(int index) {
	return children[index];
}

const std::unique_ptr<Trie::Node>& Trie::Node::find(int index) const {
	return children[index];
}

bool Trie::Node::isFinal() const {
	return final;
}

void Trie::Node::setFinal() {
	final = true;
}

Trie::Trie(Alphabet alphabet)
		: root_(alphabet, nullptr),
		  alphabet_(std::move(alphabet)) {
}

void Trie::insert(const Word& word) {
	Node *node = &root_;
	for (Word::const_iterator it = word.begin(); it != word.end(); it++) {
		const Letter& l = *it;
		int index = alphabet_.getIndex(l);
		std::unique_ptr<Node>& child = node->find(index);
		if (child == nullptr) {
			child.reset(new Node(alphabet_, node));
		}
		node = child.get();
	}
	node->setFinal();
}

bool Trie::find(const Word& word) const {
	const Node *node = &root_;
	for (Word::const_iterator it = word.begin(); it != word.end(); it++) {
		const Letter& l = *it;
		int index = alphabet_.getIndex(l);
		const std::unique_ptr<Node>& child = node->find(index);
		if (child == nullptr) {
			return false;
		} else {
			node = child.get();
		}
	}
	return node->isFinal();
}

