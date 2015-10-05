/*
 * Trie.h
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#ifndef TRIE_H_
#define TRIE_H_

#include <memory>
#include <string>
#include <vector>
#include "Alphabet.h"

class Trie {
public:
	static const long int MAX_WORD_LENGTH = 40;

	class Node {
	protected:
		const Node *parent;
		std::vector<std::unique_ptr<Node>> children;
		bool final;
	public:
		Node(const Alphabet& alphabet, const Node* parent);

		std::unique_ptr<Node>& find(int index);
		const std::unique_ptr<Trie::Node>& find(int index) const;

		const Node* getParent() const;

		const std::vector<std::unique_ptr<Trie::Node>>& getChildren() const;

		bool isFinal() const;
		void setFinal();
	};

public:
	Trie(Alphabet alphabet);
	virtual ~Trie() {};

	virtual void insert(const Word& word);

	virtual bool find(const Word& word) const;

protected:
	Node root_;
	const Alphabet alphabet_;
};

#endif /* TRIE_H_ */
