//      file.hpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#ifndef _MH_FILE_
#define _MH_FILE_

#include <iostream>
using namespace std;

class file {
	string filename;
	int mtime;
	bool existing;
	bool old;
public:
	file(string = "", int mt = 0, bool ex = false, bool o = false);
	~file();
	file & makeExisting();
	file & makeOld();
	// getters
	bool isExisting() const;
	bool isOld() const;
	string getFilename() const;
	int getMtime() const;
	// setters
	void setMtime(int);
	// comparsions should be done via filename
	friend bool operator >  (const file &, const file &);
	friend bool operator >= (const file &, const file &);
	friend bool operator <  (const file &, const file &);
	friend bool operator <= (const file &, const file &);
	friend bool operator == (const file &, const file &);
	friend bool operator != (const file &, const file &);
	// for writing to the filelist
	friend ostream & operator << (ostream &, const file &);
};

#endif
