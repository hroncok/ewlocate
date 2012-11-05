//      bitmap.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include "bitmap.hpp"

// in this file vagon refers to one integer and seat refers to one bit of it

// i - how many seats do I need? will be rounded up to vagons
bitmap::bitmap(int i) {
	ints = ((i-1)/32)+1; // gets 1 for 32, but 2 for 33
	map = new int [ints];
	for (int v = 0; v < ints; v++) {
		map[v] = 0;
	}
}

bitmap::bitmap(const bitmap & b) {
	ints = b.ints;
	map = new int[ints];
	for (int i = 0; i < ints; i++) {
		map[i] = b.map[i];
	}
}

bitmap::~bitmap() {
	delete [] map;
}

bool bitmap::in(int test) const {
	if (test >= ints*32) {
		return false;
	}
	int vagon = test / 32;
	int seat = test % 32;
	int mask = 1 << seat;
	if ((map[vagon] & mask) == 0) {
		return false;
	}
	return true;
}

bool bitmap::add(int add) {
	if (add >= ints*32) {
		return false;
	}
	int vagon = add / 32;
	int seat = add % 32;
	int mask = 1 << seat;
	if ((map[vagon] & mask) != 0) {
		return false;
	}
	map[vagon] |= mask;
	return true;
}

bool bitmap::remove(int del) {
	if (del >= ints*32) {
		return false;
	}
	int vagon = del / 32;
	int seat = del % 32;
	int mask = 1 << seat;
	if ((map[vagon] & mask) == 0) {
		return false;
	}
	map[vagon] &= (~mask);
	return true;
}

bool bitmap::empty() const {
	for (int i = 0; i < ints; i++) {
		if (map[i] != 0) {
			return false;
		}
	}
	return true;
}

bitmap & bitmap::clean() {
	for (int i = 0; i < ints; i++) {
		map[i] = 0;
	}
	return *this;
}

bitmap & bitmap::operator = (const bitmap &b) {
	if (this == &b) {
		return *this;
	}
	delete [] map;
	ints = b.ints;
	map = new int[ints];
	for (int i = 0; i < ints; i++) {
		map[i] = b.map[i];
	}
	return *this;
}
