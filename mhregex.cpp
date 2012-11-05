//      mhregex.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include "mhregex.hpp"

bool regmatch(string str, string pattern, bool icase) { 
	int status; 
	regex_t preg;
	// "compile" the expression
	if(regcomp(&preg, pattern.c_str(), REG_EXTENDED|REG_NOSUB|(REG_ICASE*!icase)) != 0) {
		return false;
	}
	// save the result
	status = regexec(&preg, str.c_str(), (size_t) 0, NULL, 0);
	// trash the expression
	regfree(&preg);
	// go!
	if(status == 0) {
		return true;
	}
	return false;
}
