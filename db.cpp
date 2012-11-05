//      db.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include "db.hpp"

db::db(int argc, char **argv) {
	loaded = false;
}

db::~db() {
	delete cf;
}

void db::vecho(string s) {
	if (cf->isVerbose()) {
		cout << s << endl;
	}
}

db & db::loadFileList() {
	if (cf->isForce()) {
		vecho("Not loading the file list (force).");
		return *this;
	}
	vecho("Loading the file list...");
	string line;
	bool fail = false;
	ifstream filelist(getPath("fl").c_str()); // it does not work with c++ string :(
	if (filelist.is_open()) {
		while (filelist.good()) {
			getline(filelist,line);
			// Ignore empty lines (probably only the last one)
			if (line.empty()) {
				continue;
			}
			size_t colon = line.find(":");
			if (colon==string::npos) {
				vecho("The file list is corrupted. The database (if any) will be recereated.");
				delFiles();
				fail = true;
				break;
			}
			string filename = line.substr(0,colon);
			int mtime;
			istringstream iss(line.substr(colon+1));
			if ((iss >> mtime).fail()) {
				vecho("The file list is corrupted. The database (if any) will be recereated.");
				delFiles();
				fail = true;
				break;
			}
			vecho("     adding "+filename);
			file tmp = file(filename, mtime);
			addFile(tmp);
			filename.clear();
		}
		filelist.close();
		if (!fail) {
			vecho("...filelist loaded.");
		}
	} else {
		vecho("The file list is not existing or unreadable. The database (if any) will be recereated.");
	}
	line.clear();
	return *this;
}

db & db::saveFileList() {
	vecho("Saving the file list...");
	ofstream filelist(getPath("fl").c_str());
	if (filelist.is_open()) {
		for (int i = 0; i < files(); i++) {
			filelist << getFile(i) << endl;
			vecho("     saving "+getFile(i).getFilename());
		}
		filelist.close();
	} else {
		perror("Error writing to filelist");
		exit(1); // this is also not very clean
	}
	vecho("...filelist saved.");
	return *this;
}

// look at the actaul file structure and comape it to filelist
db & db::index(string path) {
	if (path == "root") {
		loadFileList();
		path = getPath();
		vecho("Indexing "+path+"...");
	}
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(path.c_str())) == NULL) {
		if (path == getPath()) {
			perror("Error reading the index root, this is fatal");
			exit(1); // when I use this, valgrind is telling me stuff :D
		} else {
			perror(("Unable to open "+path+", skipping").c_str());
			return *this;
		}
	}
	while ((dirp = readdir(dp)) != NULL) {
		string fname = string(dirp->d_name);
		// skip hidden files if we should
		if ((fname.substr(0,1) == ".") && (!cf->isHidden()))
			continue;
		// skip unwanted directories (this is only happening when hidden files are indexed)
		if ((fname == "..") || (fname == ".") || (fname == ".dbaze")) {
			continue;
		}
		DIR *newdir;
		if (((newdir = opendir((path+"/"+fname).c_str())) == NULL) && (errno == 20)) {
			// it is a file
			ifstream tested((path+"/"+fname).c_str());
			if (tested.is_open()) {
				// ASCII test (kind of stupid on long files, testing just a few first chars would be better)
				int c;
				while (((c = tested.get()) != EOF) && (c <= 127));
				if (c != EOF) {
					tested.close();
					continue;
				}
				struct stat fstat;
				stat((path+"/"+fname).c_str(), &fstat);
				file * f = new file(path+"/"+fname,fstat.st_mtime,true,true);
				int id = addFile(*f);
				// here it is tricky, we have an position where is our new file or an old copy
				delete f;
				if (getFile(id).isExisting()) {
					// it is our new file
					vecho("     adding "+path+"/"+fname);
					tested.close();
					continue;
				}
				// it is an old copy
				getFile(id).makeExisting();
				// should be indexed again?
				stat((path+"/"+fname).c_str(), &fstat);
				if (getFile(id).getMtime() < fstat.st_mtime) {
					getFile(id).makeOld();
				}
				tested.close();
			} else {
				perror(("Unable to open "+path+"/"+fname+", skipping").c_str());
			}
		} else {
			// it is a dir
			closedir(newdir);
			char buf[1];
			if (readlink((path+"/"+fname).c_str(), buf, 1) == -1) { // it is not a symlink
				index(path+"/"+fname);
			}
		}
		fname.clear();
	}
	closedir(dp);
	if (path == getPath()) {
		removeNonExisting();
		indexOld();
		vecho("...directory indexed.");
	}
	return *this;
}

db & db::save() {
	if ((loaded) || (cf->isForce())) {
		mkdir(getPath("dir").c_str(),S_IRWXU);
		saveFileList();
		saveWords();
	}
	return *this;
}

// this file shoud be disappeared from all the words
db & db::vaporizeFile(int id) {
	word ** arr = wordsArray();
	for (int i = 0; arr[i]; i++) {
		arr[i]->rmFile(id);
		arr[i] = NULL;
	}
	delete [] arr;
	return *this;
}

db & db::indexFile(int id) {
	vecho("     indexing "+getFile(id).getFilename());
	ifstream indexed(getFile(id).getFilename().c_str());
	if (indexed.is_open()) {
		char c;
		string term = "";
		char offset = 'a'-'A';
		if (cf->isCaseSensitive()) {
			offset = 0;
		}
		do {
			c = indexed.get();
			if (islower(c)) {
				term += c;
			} else if (isupper(c)) {
				term += (c+offset);
			} else if ((!cf->isNumSeparate()) && (isdigit(c))) { // could be merged with first if, but this makes more sence
				term += c;
			} else { // got separator
				if (term.empty()) {
					continue;
				}
				word * tmp = new word(files(),term);
				if (!addWord(tmp)) { // was there already
					delete tmp;
					tmp = getWord(term);
				}
				tmp->addFile(id);
				tmp = NULL;
				vecho("         "+term);
				term = "";
			}
		} while (c != EOF);
		indexed.close();
		term.clear();
	} else {
		perror("Error reading file");
	}
	return *this;
}

db & db::removeNonExisting() {
	for (int i = 0; i < files(); i++) {
		if (!getFile(i).isExisting()) {
			loadWords();
			vaporizeFile(i);
			vecho("     removing "+getFile(i).getFilename());
			rmFile(i);
		}
	}
	return *this;
}

db & db::indexOld() {
	for (int i = 0; i < files(); i++) {
		if (getFile(i).isOld()) {
			loadWords();
			vaporizeFile(i);
			indexFile(i);
		}
	}
	return *this;
}

int db::addWord(word * w) {
	return words.add(w);
}

bool db::rmWord(word * w) {
	return words.remove(w);
}

bool db::noWords() const {
	return words.empty();
}

word * db::getWord(string s) const {
	return words.fromString(s);
}

word ** db::wordsArray() const {
	return words.toArray();
}

int db::addFile(file &f) {
	return allFiles.aog(f);
}

void db::rmFile(int id) {
	return allFiles.removeID(id);
}

file & db::getFile(int i) {
	return allFiles.get(i);
}

int db::files() {
	return allFiles.size();
}

set<file> & db::delFiles() {
	return allFiles.erase();
}

string db::getPath(string s) const {
	if (s == "root") {
		return cf->getPath();
	}
	if (s == "dir") {
		return cf->getPath("dir");
	}
	if (s == "fl") {
		return cf->getPath("dir")+"/filelist";
	}
	if (s == "db") {
		return cf->getPath("dir")+"/db";
	}
	return "";
}
