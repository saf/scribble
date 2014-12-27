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
private:
	class Node {
	private:
		Node *parent;
		std::vector<Node *> *children;
		bool final;
	public:
		Node(int childCount);
		~Node();

		void insert(int index, Node *child);
		Node *find(int index);

		Node *getParent();

		bool isFinal();
		void setFinal();
	};

	Node root;
	Alphabet *alphabet;
public:
	Trie(Alphabet &alphabet);
	virtual ~Trie();

	void insert(std::wstring &word);
	bool find(std::wstring &word);
};

#endif /* TRIE_H_ */
