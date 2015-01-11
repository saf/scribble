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

protected:
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

		Node *getParent() const;

		bool isFinal();
		void setFinal();
	};

	Node root;
	const Alphabet *alphabet;

	int lastInsertedLength;
	Node *lastInsertedNode;
	wchar_t lastInsertedWord[MAX_WORD_LENGTH];
public:

	Trie(const Alphabet &alphabet);
	virtual ~Trie();

	void insert(std::wstring &word);
	void insert(const wchar_t *word);

	bool find(std::wstring &word);
	bool find(const wchar_t *word);
};

#endif /* TRIE_H_ */
