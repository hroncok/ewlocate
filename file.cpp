//      file.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include "file.hpp"

file::file(string f, int mt, bool e, bool o) {
	filename = f;
	existing = e;
	mtime = mt;
	old = o;
}

file::~file() {
	filename.clear();
}

file & file::makeExisting() {
	existing = true;
	return *this;
}

file & file::makeOld() {
	old = true;
	return *this;
}

bool file::isExisting() const {
	return existing;
}

bool file::isOld() const {
	return old;
}

string file::getFilename() const {
	return filename;
}

int file::getMtime() const {
	return mtime;
}

void file::setMtime(int mt) {
	mtime = mt;
}

ostream & operator << (ostream &out, const file &x) {
	out << x.filename << ":" << x.mtime;
	return out;
}

bool operator >  (const file &a, const file &b) {
	return (a.filename >  b.filename);
}

bool operator >= (const file &a, const file &b) {
	return (a.filename >= b.filename);
}

bool operator <  (const file &a, const file &b) {
	return (a.filename <  b.filename);
}

bool operator <= (const file &a, const file &b) {
	return (a.filename <= b.filename);
}

bool operator == (const file &a, const file &b) {
	return (a.filename == b.filename);
}

bool operator != (const file &a, const file &b) {
	return (a.filename != b.filename);
}
