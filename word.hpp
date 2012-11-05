//      word.hpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#ifndef _MH_WORD_
#define _MH_WORD_

#include <iostream>
using namespace std;
#include "bitmap.hpp"

class word {
	string term;
	bitmap files;
public:
	word(int = 32, string = "");
	~word();
	word & operator = (const word &);
	// getters
	string getTerm() const;
	// operations on set
	int addFile(int);
	bool rmFile(int);
	bool in(int) const;
	bool noFile() const;
	// comparsions should be done via term
	friend bool operator >  (const word &, const word &);
	friend bool operator >= (const word &, const word &);
	friend bool operator <  (const word &, const word &);
	friend bool operator <= (const word &, const word &);
	friend bool operator == (const word &, const word &);
	friend bool operator != (const word &, const word &);
};

#endif
