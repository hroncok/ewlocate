//      wdb.hpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#ifndef _MH_WDB_
#define _MH_WDB_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;
#include "db.hpp"

class wdb : public db {
protected:
	virtual wdb & loadWords();
	virtual wdb & saveWords();
	wdb & delWords();
public:
	virtual wdb & search();
	wdb(int argc = 0, char **argv = NULL, bool search = false);
};

// useful procedure, doesn't belong to this class
void tolower(string &);
// could be also done this way, but I'll never need it in my code
// string tolower(string);

#endif
