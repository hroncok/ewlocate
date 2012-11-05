//      bitmap.hpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#ifndef _MH_BITMAP_
#define _MH_BITMAP_

#include <iostream>
using namespace std;

class bitmap {
	int * map;
	int ints;
public:
	bitmap(int = 32);
	bitmap(const bitmap &);
	~bitmap();
	bool in(int) const;
	bool add(int);
	bool remove(int);
	bool empty() const;
	bitmap & clean();
	bitmap & operator = (const bitmap &);
};

#endif