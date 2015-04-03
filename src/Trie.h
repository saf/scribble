/*
 * Trie.h
 *
 *  Created on: 27-12-2014
 *      Author: saf
 */

#ifndef TRIE_H_
#define TRIE_H_

#include <string>
#include <vector>
#include "Alphabets.h"

class Trie {
public:
	static const long int MAX_WORD_LENGTH = 40;

	class Node {
	protected:
		Node *parent;
		std::vector<Node *> *children;
		bool final;
	public:
		Node(int childCount, Node *parent = NULL);
		~Node();

		void insert(int index, Node *child);

		Node *find(int index);
		const Node *find(int index) const;

		Node *getParent();
		const Node *getParent() const;

		const std::vector<Node *>& getChildren() const;

		bool isFinal() const;
		void setFinal();
	};

protected:
	Node root;
	const Alphabet *alphabet;

	int lastInsertedLength;
	Node *lastInsertedNode;
	wchar_t lastInsertedWord[MAX_WORD_LENGTH];
public:

	Trie(const Alphabet &alphabet);
	virtual ~Trie();

	virtual void insert(std::wstring &word);
	virtual void insert(const wchar_t *word);

	virtual bool find(std::wstring &word) const;
	virtual bool find(const wchar_t *word) const;
};

#endif /* TRIE_H_ */
