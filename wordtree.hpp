//      wordtree.hpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>


#include <iostream>
using namespace std;
#include "word.hpp"

class wordtree {
//protected:
	const static int l = 2*26+10; // a-zA-Z0-9
	struct branch {
		branch ** abc; // one pointer for each abc
		word * val;
		branch();
		~branch();
		branch * clone() const;
	};
	branch * start;
	int pos(char) const;
	void subArray(word ** &, branch *, int &, int &) const;
	int makeBigger(word ** &, int cs, int ds = 0) const;
public:
	wordtree();
	~wordtree();
	wordtree & operator = (const wordtree &);
	bool add(word *);
	bool remove(word *);
	bool empty() const;
	word * fromString(string) const;
	wordtree & erase();
	word ** toArray() const;
};

/* Doesn't work that way :(
// wordtree with small letters only
class smalltree : public wordtree {
protected:
	const static int l = 26;
	virtual int pos(char) const;
};

// wordtree with small and capital letters
class bigtree : public wordtree {
protected:
	const static int l = 26*2;
	virtual int pos(char) const;
};

...
*/
