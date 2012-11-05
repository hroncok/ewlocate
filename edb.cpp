//      edb.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include "edb.hpp"

edb::edb(int argc, char **argv, bool search) : db(argc, argv) {
	if (search) {
		cf = new sconfig('e', argc, argv); // config for searching
	} else {
		cf = new iconfig('e', argc, argv); // config for indexing
	}
}

edb & edb::loadWords() {
	if ((loaded) || (cf->isForce())) {
		return *this;
	}
	vecho("Loading words...");
	string line = "";
	ifstream wordlist(getPath("db").c_str());
	if (wordlist.is_open()) {
		if (!wordlist.good()) { // load the first one
			return *this; // empty file
		}
		while (wordlist.good()) {
			if (line.empty()) {
				getline(wordlist,line);
			}
			if (line[0] != ':') {
				cerr << "Wordlist mishmash, try to use --force.";
				exit(1);
				}
			line = line.substr(1);
			vecho("     reading "+line);
			word * newWord = new word(files(),line);
			while (wordlist.good()) {
				getline(wordlist,line);
				if (line.empty()) {
					continue; // ignore empty lines, especially the last one
				}
				if (line[0] != ':') {
					file tmp = file(line);
					newWord->addFile(addFile(tmp));
				} else {
					break;
				}
			}
			addWord(newWord);
			newWord = NULL;
		}
		wordlist.close();
		vecho("...words loaded.");
	} else {
		vecho("Error reading wordlist. Maybe this is the first run?");
	}
	loaded = true;
	line.clear();
	return *this;
}

edb & edb::saveWords() {
	vecho("Saving words...");
	ofstream wordlist(getPath("db").c_str());
	if (wordlist.is_open()) {
		word ** arr = wordsArray();
		for (int i = 0; arr[i]; i++) {
			if (!arr[i]->noFile()) {
				vecho("     saving "+arr[i]->getTerm());
				wordlist << ":" << arr[i]->getTerm() << endl;
				for (int f = 0; f < files(); f++) {
					if (arr[i]->in(f)) {
						wordlist << getFile(f).getFilename() << endl;
						vecho("        in "+getFile(f).getFilename());
					}
				}
			}
			arr[i] = NULL;
		}
		wordlist.close();
		delete [] arr;
	} else {
		perror("Error writing to wordlist");
		exit(1); // this is also not very clean
	}
	vecho("...words saved.");
	return *this;
}

/*
// This way is celan, but not very effective, cause wordtree is stupidly done and undone
edb & edb::search() {
	loadWords();
	word ** arr = wordsArray();
	bool found = false;
	for (int i = 0; arr[i]; i++) {
		if (regmatch(arr[i]->getTerm(),cf->getTerm(),cf->isCaseSensitive())) {
			cout << arr[i]->getTerm() << ":" << endl;
			for (int f = 0; f < files(); f++) {
				if (arr[i]->in(f)) {
					cout << "  " << getFile(f).getFilename() << endl;
					found = true;
				}
			}
		}
		arr[i] = NULL;
	}
	delete [] arr;
	if (!found) {
		cerr << "No match." << endl;
		exit(2);
	}
	return *this;
}
*/

// This is very similar to loadWords(), but instead of saving to struture, it is shown
edb & edb::search() {
	string line;
	// easter egg
	if (regmatch(cf->getTerm(),"chuck|norris",false)) {
		cout << "This app won't search for Chuck Norris because it knows you don't find Chuck Norris, he finds you." << endl;
		cout << "Type YES (case sensitive) if you want to continue anyway: " << flush;
		getline(cin,line);
		if (line != "YES") {
			exit(3);
		}
		line = "";
	}
	// end of easter egg
	bool found = false;
	bool now = false;
	ifstream wordlist(getPath("db").c_str());
	if (wordlist.is_open()) {
		if (!wordlist.good()) { // empty file
			cerr << "No match." << endl;
			exit(2);
		}
		while (wordlist.good()) {
			if (line.empty()) {
				getline(wordlist,line);
			}
			if (line[0] != ':') {
				cerr << "Wordlist mishmash, reindex with --force." << endl;
				exit(1);
			}
			line = line.substr(1);
			if (regmatch(line,cf->getTerm(),cf->isCaseSensitive())) {
				now = found = true;
				found = true;
				cout << line << ":" << endl;
			}
			while (wordlist.good()) {
				getline(wordlist,line);
				if (line.empty()) {
					continue; // ignore empty lines, especially the last one
				}
				if (line[0] != ':') {
					if (now) {
						cout << "  " << line << endl;
					}
				} else {
					break;
				}
			}
			now = false;
		}
		wordlist.close();
	} else {
		cerr << "Error reading wordlist." << endl;
		exit(1);
	}
	if (!found) {
		cerr << "No match." << endl;
		exit(2);
	}
	line.clear();
	return *this;
}
