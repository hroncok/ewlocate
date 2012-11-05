//      edbzer.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>
//      

#include <iostream>
#include "edb.hpp"
using namespace std;

int main(int argc, char **argv) {
	edb dbaze = edb(argc, argv);
	dbaze.index().save();
	return 0;
}
