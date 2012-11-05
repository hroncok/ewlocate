//      mhregex.hpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>
//      This file just brings me an easy regular expression match function

#ifndef _MH_REGEX_
#define _MH_REGEX_

#include <string>
#include <regex.h>
using namespace std;

bool regmatch(string str, string pattern, bool icase = true);

#endif
