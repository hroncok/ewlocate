//      db.hpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#ifndef _MH_DB_
#define _MH_DB_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;
#include "set.cpp"
#include "file.hpp"
#include "word.hpp"
#include "wordtree.hpp"
#include "config.hpp"

class db {
protected:
	config * cf;
	wordtree words;
	set<file> allFiles;
	bool loaded;
	void vecho(string);
	virtual db & loadWords() = 0;
	virtual db & saveWords() = 0;
	db & vaporizeFile(int);
	db & indexFile(int);
	db & removeNonExisting();
	db & indexOld();
	db & loadFileList();
	db & saveFileList();
	// oprations on tree
	int addWord(word *);
	bool rmWord(word *);
	bool noWords() const;
	word * getWord(string) const;
	word ** wordsArray() const;
	// operations on set
	int addFile(file &);
	void rmFile(int);
	file & getFile(int);
	int files();
	set<file> & delFiles();
	string getPath(string = "root") const;
public:
	db(int argc = 0, char **argv = NULL);
	~db();
	db & index(string = "root");
	db & save();
	virtual db &  search() = 0;
};

#endif
