//      word.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include "word.hpp"

word::word(int v, string w) : files(v) {
	term = w;
}

word::~word() {
	term.clear();
}

word & word::operator = (const word &w) {
	if (this == &w) {
		return *this;
	}
	term = w.term;
	files = w.files;
	return *this;
}

string word::getTerm() const {
	return term;
}

int word::addFile(int f) {
	files.add(f);
	return f;
}

bool word::rmFile(int f) {
	return files.remove(f);
}

bool word::noFile() const {
	return files.empty();
}

bool word::in(int f) const {
	return files.in(f);
}

bool operator >  (const word &a, const word &b) {
	return (a.term >  b.term);
}

bool operator >= (const word &a, const word &b) {
	return (a.term >= b.term);
}

bool operator <  (const word &a, const word &b) {
	return (a.term <  b.term);
}

bool operator <= (const word &a, const word &b) {
	return (a.term <= b.term);
}

bool operator == (const word &a, const word &b) {
	return (a.term == b.term);
}

bool operator != (const word &a, const word &b) {
	return (a.term != b.term);
}
