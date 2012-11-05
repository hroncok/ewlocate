//      wdb.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include "wdb.hpp"

wdb::wdb(int argc, char **argv, bool search) : db(argc, argv) {
	if (search) {
		cf = new sconfig('w', argc, argv); // config for searching
	} else {
		cf = new iconfig('w', argc, argv); // config for indexing
	}
}

wdb & wdb::loadWords() {
	if ((loaded) || (cf->isForce())) {
		return *this;
	}
	vecho("Loading words...");
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(getPath("db").c_str())) == NULL) {
		vecho("Error reading the database root, maybe this is the first run");
	} else {
		while ((dirp = readdir(dp)) != NULL) {
			string term = string(dirp->d_name);
			// skip hidden files
			if (term.substr(0,1) == ".") {
				continue;
			}
			ifstream wordfile((getPath("db")+"/"+term).c_str());
			if (wordfile.is_open()) {
				word * newWord = new word(files(),term);
				string line;
				while (wordfile.good()) {
					getline(wordfile,line);
					// Ignore empty lines (probably only the last one)
					if (line.empty()) {
						continue;
					}
					file tmp = file(line);
					newWord->addFile(addFile(tmp));
				}
				wordfile.close();
				addWord(newWord);
				newWord = NULL;
				vecho("     reading "+term);
			} else {
				perror("Error reading file");
			}
			term.clear();
		}
		closedir(dp);
	}
	loaded = true;
	vecho("...words loaded.");
	return *this;
}

wdb & wdb::delWords() {
	vecho("Deleting old words...");
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(getPath("db").c_str())) == NULL) {
		perror("Unable to open the database root, maybe this is the first run");
	} else {
		while ((dirp = readdir(dp)) != NULL) {
			string term = string(dirp->d_name);
			// skip hidden files
			if (term.substr(0,1) == ".") {
				continue;
			}
			if (remove((getPath("db")+"/"+term).c_str()) != 0 ) {
				perror("Error deleting");
			} else {
				vecho("     deleting "+term);
			}
			term.clear();
		}
		closedir(dp);
	}
	vecho("...old words deleted.");
	return *this;
}

wdb & wdb::saveWords() {
	mkdir(getPath("db").c_str(),S_IRWXU);
	delWords();
	vecho("Saving words...");
	word ** arr = wordsArray();
	for (int i = 0; arr[i]; i++) {
		if (!arr[i]->noFile()) {
			vecho("     saving "+arr[i]->getTerm());
			ofstream wordfile((getPath("db")+"/"+arr[i]->getTerm()).c_str());
			for (int f = 0; f < files(); f++) {
				if (arr[i]->in(f)) {
					wordfile << getFile(f).getFilename() << endl;
					vecho("        in "+getFile(f).getFilename());
				}
			}
		}
		arr[i] = NULL;
	}
	delete [] arr;
	vecho("...words saved.");
	return *this;
}


wdb & wdb::search() {
	string term = cf->getTerm();
	if (!cf->isCaseSensitive()) {
		tolower(term);
	}
	string line;
	ifstream wordfile((getPath("db")+"/"+term).c_str());
	if (wordfile.is_open()) {
		while (wordfile.good()) {
			getline(wordfile,line);
			// Ignore empty lines (probably only the last one)
			if (line.empty()) {
				continue;
			}
			cout << line << endl;
		}
		wordfile.close();
	} else {
		cerr << "No match." << endl;
		exit(2);
	}
	line.clear();
	term.clear();
	return *this;
}

void tolower(string & s) {
	for (size_t i = 0; i < s.length(); i++) {
		s[i] = tolower(s[i]);
	}
}

/* as said in .hpp, this is also an option:
string tolower(string s) {
	for (size_t i = 0; i < s.length(); i++) {
		s[i] = tolower(s[i]);
	}
	return s;
}
*/
