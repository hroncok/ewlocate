//      set.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>
//      
//      Because templates are compiled when required, this forces a restriction for multi-file projects:
//      the implementation (definition) of a template class or function must be in the same file as its
//      declaration. That means that we cannot separate the interface in a separate header file, and that
//      we must include both interface and implementation in any file that uses the templates.
//          from http://www.cplusplus.com/doc/tutorial/templates/

#ifndef _MH_SET_
#define _MH_SET_

#include <iostream>
#include <cstdlib>
using namespace std;

// INTERFACE

template <class item>
class set {
	item* items; // array of items
	int usize; // used size of items
	int rsize; // real size of items
	set<item> &makeBigger();
	int whereIs(item, int, int) const;
public:
	set(int = 16);
	set<item> & operator = (const set<item> &);
	~set();
	int aog(item &); // add or get
	bool remove(item);
	void removeID(int);
	bool empty() const;
	set<item> & erase(int = 128);
	// getters
	int size() const;
	item & get(int) const;
};

// IMPLEMENTATION

template <class item>
set<item>::set(int len) {
	if (len < 1) {
		rsize=1;
	}
	else {
		rsize = len;
	}
	usize = 0;
	items = new item[len];
}

template <class item>
set<item> & set<item>::operator = (const set<item> &s) {
	if (this == &s) {
		return *this;
	}
	delete [] items;
	usize = s.usize;
	rsize = s.rsize;
	items = new item[rsize];
	for (int i = 0; i < usize; i++) {
		items[i] = s.items[i];
	}
	return *this;
}

template <class item>
set<item>::~set() {
	delete [] items;
}

template <class item>
set<item> & set<item>::makeBigger() {
	rsize *= 2;
	item * items2 = new item[rsize];
	for (int i=0; i<usize; i++) {
		items2[i] = items[i];
	}
	delete [] items;
	items = items2;
	items2 = NULL;
	return *this;
}

template <class item>
int set<item>::whereIs(item I, int start, int end) const {
	int len = end - start;
	if (len == 0) {
		if (I < items[start]) {
			return start;
		} else {
			return start+1;
		}
	}
	if (len == 1) {
		if (I < items[start]) {
			return start;
		} else {
			return end;
		}
	}
	int mid = (len+1)/2;
	if (I < items[start+mid]) {
		return whereIs(I, start, start+mid);
	}
	if (I > items[start+mid]) {
		return whereIs(I, start+mid, end);
	}
	return start+mid;
}

template <class item>
int set<item>::aog(item & I) {
	if (empty()) {
		items[0] = I;
		usize = 1;
		return 0;
	}
	if (usize==rsize) {
		makeBigger();
	}
	if (I > items[usize-1]) {
		// to the end
		items[usize++] = I;
		return usize-1;
	}
	if (I == items[usize-1]) {
		// it is the last one
		return usize-1;
	}
	if (I == items[0]) {
		// it is the first one
		return 0;
	}
	int id;
	if (I < items[0]) {
		// to the beg
		id = 0;
	} else {
		id = whereIs(I, 0, usize-1);
		if (I == items[id]) {
			return id;
		}
	}
	item tmp1 = I;
	for (int i=id; i<usize; i++) {
		item tmp2 = items[i];
		items[i] = tmp1;
		tmp1 = tmp2;
	}
	items[usize++] = tmp1;
	return id;
}

template <class item>
bool set<item>::remove(item I) {
	int id = whereIs(I, 0, usize-1);
	if (items[id] != I) {
		return false;
	}
	removeID(id);
	return true;
}

template <class item>
void set<item>::removeID(int id) {
	for (int i=id; i<(usize-1); i++) {
		items[i] = items[i+1];
	}
	usize--;
}

template <class item>
bool set<item>::empty() const {
	return (usize == 0);
}

template <class item>
set<item> & set<item>::erase(int size) {
	delete [] items;
	rsize = size;
	usize = 0;
	items = new item[rsize];
	return *this;
}

template <class item>
int set<item>::size() const {
	return usize;
}

template <class item>
item & set<item>::get(int i) const {
	return items[i];
}

#endif
