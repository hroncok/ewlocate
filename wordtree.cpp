//      wordtree.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include "wordtree.hpp"

wordtree::branch::branch() {
	abc = new branch * [l];
	for (int i = 0; i < l; i++) {
		abc[i] = NULL;
	}
	val = NULL;
}

wordtree::branch::~branch() {
	for (int i = 0; i < l; i++) {
		if (abc[i]) {
			delete abc[i];
		}
	}
	delete [] abc;
	if (val) {
		delete val;
	}
}

wordtree::branch * wordtree::branch::clone() const {
	branch * c = new branch;
	for (int i = 0; i < l; i++) {
		if (abc[i]) {
			c->abc[i] = abc[i]->clone();
		} else {
			c->abc[i] = NULL;
		}
	}
	return c;
}

wordtree::wordtree() {
	start = NULL;
}

wordtree::~wordtree() {
	if (start) {
		delete start;
	}
}

wordtree & wordtree::operator = (const wordtree & w) {
	if (this == &w) {
		return *this;
	}
	delete start;
	if (w.start) {
		start = w.start->clone();
	} else {
		start = NULL;
	}
	return *this;
}

// translate characters to array indexes
int wordtree::pos(char c) const {
	if (islower(c)) {
		return (c - 'a');
	}
	if (isupper(c)) {
		return (c - 'A' + 26);
	}
	return (c - '0' + 26*2);
}

bool wordtree::add(word * w) {
	string tmp = w->getTerm();
	if (tmp.empty()) {
		return false;
	}
	if (!start) {
		start = new branch;
	}
	branch * mov = start;
	while (!tmp.empty()) {
		int id = pos(tmp[0]);
		tmp = tmp.substr(1);
		if (!mov->abc[id]) {
			mov->abc[id] = new branch;
		}
		mov = mov->abc[id];
	}
	// mov is now on the right place
	if (mov->val) {
		return false; // something allready sits here
	}
	mov->val = w;
	tmp.clear();
	return true;
}

bool wordtree::remove(word * w) {
	string tmp = w->getTerm();
	if (tmp.empty()) {
		return false;
	}
	if (!start) {
		return false;
	}
	branch * mov = start;
	int id;
	while (!tmp.empty()) {
		id = pos(tmp[0]);
		tmp = tmp.substr(1);
		mov = mov->abc[id];
		if (!mov) {
			return false;
		}
	}
	// mov is now on the right place
	if (!mov->val) {
		return false; // nothing sits here
	}
	delete mov->val;
	mov->val = NULL;
	tmp.clear();
	// no reason to clean the structure
	return true;
}

bool wordtree::empty() const {
	return (start == NULL);
}

word * wordtree::fromString(string term) const {
	if (term.empty()) {
		return NULL;
	}
	if (!start) {
		return NULL;
	}
	branch * mov = start;
	int id;
	while (!term.empty()) {
		id = pos(term[0]);
		term = term.substr(1);
		mov = mov->abc[id];
		if (!mov) {
			return NULL;
		}
	}
	// mov is now on the right place
	if (!mov->val) {
		return NULL; // nothing sits here
	}
	return mov->val;
}

wordtree & wordtree::erase() {
	if (!start) {
		return *this;
	}
	delete start;
	start = NULL;
	return *this;
}

int wordtree::makeBigger(word ** & arr, int cs, int ds) const {
	if (ds == 0) {
		ds = 2*cs;
	}
	word ** arr2 = new word * [ds];
	for (int i = 0; i < cs; i++) {
		arr2[i] = arr[i];
	}
	delete [] arr;
	arr = arr2;
	arr2 = NULL;
	return ds;
}

void wordtree::subArray(word ** & arr, branch * b, int & cs, int & ds) const {
	if (b->val) {
		if (ds == cs) {
			ds = makeBigger(arr,cs);
		}
		arr[cs++] = b->val;
	}
	for (int i = 0; i < l; i++) {
		if (b->abc[i]) {
			subArray(arr, b->abc[i], cs, ds);
		}
	}
}

// transform the tree to an array, so everybody can work with the words
word ** wordtree::toArray() const {
	if (empty()) {
		word ** arr = new word * [1];
		arr[0] = NULL;
		return arr;
	}
	int ds = 128;
	int cs = 0;
	word ** arr = new word * [ds];
	for (int i = 0; i < l; i++) {
		if (start->abc[i]) {
			subArray(arr, start->abc[i], cs, ds);
		}
	}
	if (cs == 0) {
		delete [] arr;
		word ** arr = new word * [1];
		arr[0] = NULL;
		return arr;
	}
	if (ds == cs) {
		ds = makeBigger(arr,cs,cs+1);
	}
	arr[cs++] = NULL;
	return arr;
}

/* Tests
int main(int argc, char **argv) {
	wordtree wt = wordtree();
	word * w = new word(10,"1");
	cout << wt.add(w) << endl;
	w = new word(10,"b");
	cout << wt.add(w) << endl;
	word ** arr = wt.toArray();
	for (int i = 0; arr[i]; i++) {
		cout << arr[i]->getTerm() << endl;
	}
}
*/
