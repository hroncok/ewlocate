//      wdbzer.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>
//      

#include <iostream>
#include "wdb.hpp"
using namespace std;

int main(int argc, char **argv) {
	wdb dbaze = wdb(argc, argv);
	dbaze.index().save();
	return 0;
}
