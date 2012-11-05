//      config.hpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#ifndef _MH_CONFIG_
#define _MH_CONFIG_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>
using namespace std;

class config {
protected:
	string root; // root folder
	bool hidden; // should the hidden files be indexed?
	bool caseSensitive; // should the system be case sensitive?
	bool numSeparate; // are the numbers separators?
	bool verbose; // should the program has an output?
	bool force; // should the old database be ignored?
	string term; // searched term if needed
	//virtual void getArguments(int argc, char **argv) = 0; // not needed here, not a public method nor called from config  native methods (but it works with it)
	//virtual void showHelp() = 0; // dtto
	void setRoot(char *);
	char prefix;
public:
	config(char);
	virtual ~config();
	// getters
	string getPath(string = "root") const;
	bool isHidden() const;
	bool isCaseSensitive() const;
	bool isNumSeparate() const;
	bool isVerbose() const;
	bool isForce() const;
	string getTerm() const;
};

class iconfig : public config {
protected:
	void getArguments(int argc, char **argv);
	void showHelp();
	void saveSensitivity();
public:
	iconfig(char, int argc = 0, char **argv = NULL);
	virtual ~iconfig();
};

class sconfig : public config {
protected:
	void getArguments(int argc, char **argv);
	void showHelp();
	void checkRoot();
	void loadSensitivity();
public:
	sconfig(char, int argc = 0, char **argv = NULL);
	virtual ~sconfig();
};

#endif
