//      edb.hpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#ifndef _MH_EDB_
#define _MH_EDB_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
#include "mhregex.hpp"
#include "db.hpp"

class edb : public db {
protected:
	virtual edb & loadWords();
	virtual edb & saveWords();
public:
	virtual edb & search();
	edb(int argc = 0, char **argv = NULL, bool search = false);
};

#endif
