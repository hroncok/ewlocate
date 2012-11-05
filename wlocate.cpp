//      wlocate.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include <iostream>
#include "wdb.hpp"
using namespace std;

int main(int argc, char **argv) {
	wdb dbaze = wdb(argc, argv, true);
	dbaze.search();
	return 0;
}
