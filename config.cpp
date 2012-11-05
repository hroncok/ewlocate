//      config.cpp
//      
//      Copyright 2011 Miro Hrončok <miro@hroncok.cz>

#include "config.hpp"

config::config(char p) {
	// default options
	root = getenv("HOME");
	hidden = false;
	caseSensitive = false;
	verbose = false;
	force = false;
	numSeparate = false;
	prefix = p;
}

iconfig::iconfig(char p, int argc, char **argv) : config(p) {
	getArguments(argc, argv);
}

sconfig::sconfig(char p, int argc, char **argv) : config(p) {
	getArguments(argc, argv);
	checkRoot();
	loadSensitivity(); // loading befor everything is done
}

config::~config() {
	root.clear();
	term.clear();
}

iconfig::~iconfig() {
	saveSensitivity(); // saving after everything is done right
}

sconfig::~sconfig() {
	// nothing
}

// this gives me pathes to used files
string config::getPath(string s) const {
	if (s == "root") {
		return root;
	}
	if (s == "cs") {
		return getPath("dir")+"/icase";
	}
	return root+"/."+prefix+"dbaze";
}

// save the given root, but ionly if it exists
void config::setRoot(char * r) {
	char * rpath = realpath(r,NULL);
	if (rpath == NULL) {
		cerr << "Specified root is not exisiting" << endl;
		free(rpath);
		exit(1);
	}
	root = string(rpath);
	free(rpath);
}

// for searching - check if the root was indexed
void sconfig::checkRoot() {
	char * rpath = realpath(getPath("dir").c_str(),NULL);
	if (rpath == NULL) {
		cerr << "Given or default root is not indexed, run " << prefix << "dbzer" << endl;
		free(rpath);
		exit(1);
	}
	free(rpath);
}

bool config::isHidden() const {
	return hidden;
}

bool config::isCaseSensitive() const {
	return caseSensitive;
}

bool config::isNumSeparate() const {
	return numSeparate;
}

bool config::isVerbose() const {
	return verbose;
}

bool config::isForce() const {
	return force;
}

string config::getTerm() const {
	return term;
}

void iconfig::showHelp() {
	cout << "  --help or -h" << endl;
	cout << "    Displays this text." << endl << endl;
	cout << "  --root or -r" << endl;
	cout << "    Sets the index root, your home directory is default. Every root has it own database." << endl << endl;
	cout << "  --case-sensitive or -c" << endl;
	cout << "    If used, all the process is case sensitive, insensitive is default." << endl << endl;
	cout << "  --num-separate or -n" << endl;
	cout << "    If used, numbers are considered as separators, default they are considered as letters." << endl << endl;
	cout << "  --hidden or -i" << endl;
	cout << "    If used, hidden files are indexed and hidden directories are explored, default ignores both." << endl << endl;
	cout << "  --force or -f" << endl;
	cout << "    Don't update previously created database, create a new one. Don't forget to use this option when changing three above options." << endl << endl;
	cout << "  --verbose or -v" << endl;
	cout << "    Prints loads of information to standard output. Might waste time." << endl;
	exit(0);
}

void sconfig::showHelp() {
	cout << "One of the arguments ought to be the word to find." << endl << endl;
	cout << "  --help or -h" << endl;
	cout << "    Displays this text." << endl << endl;
	cout << "  --root or -r" << endl;
	cout << "    Sets the index root, your home directory is default. Every root has it own database." << endl << endl;
	cout << "By design, you cannot change --case-sensitive, --hidden or --num-separate when searching." << endl;
	exit(0);
}

void iconfig::getArguments(int argc, char **argv) {
	int c;
	while (true) {
		static struct option long_options[] = {
			{"help",			no_argument,		0,	'h'},
			{"root",			required_argument,	0,	'r'},
			{"hidden",			no_argument,		0,	'i'},
			{"case-sensitive",	no_argument,		0,	'c'},
			{"num-separate",	no_argument,		0,	'n'},
			{"verbose",			no_argument,		0,	'v'},
			{"force",			no_argument,		0,	'f'},
			{0, 0, 0, 0}
		};
		
		// getopt_long stores the option index here
		int option_index = 0;
		
		c = getopt_long(argc, argv, "hr:icnvf", long_options, &option_index);
		
		// detect the end of the options
		if (c == -1) {
			break;
		}
		
		switch (c) {
			case 'h':
				showHelp();
				break;
			
			case 'r':
				setRoot(optarg);
				break;
			
			case 'i':
				hidden = true;
				break;
			
			case 'c':
				caseSensitive = true;
				break;
			
			case 'n':
				numSeparate = true;
				break;
			
			case 'v':
				verbose = true;
				break;
			
			case 'f':
				force = true;
				break;
			
			case '?':
				// getopt_long already printed an error message
				break;
			
			default:
				abort();
		}
	}
	
	if (optind < argc) {
		cerr << "No arguments required when indexing" << endl;
		exit(1);
	}
}

void sconfig::getArguments(int argc, char **argv) {
	int c;
	while (true) {
		static struct option long_options[] = {
			{"help",			no_argument,		0,	'h'},
			{"root",			required_argument,	0,	'r'},
			/*{"verbose",			no_argument,		0,	'v'},*/
			{0, 0, 0, 0}
		};
		
		// getopt_long stores the option index here
		int option_index = 0;
		
		c = getopt_long(argc, argv, "hr:v", long_options, &option_index);
		
		// detect the end of the options
		if (c == -1) {
			break;
		}
		
		switch (c) {
			case 'h':
				showHelp();
				break;
			
			case 'r':
				setRoot(optarg);
				//checkRoot();
				break;
			
			/*case 'v':
				verbose = true;
				break;*/
			
			case '?':
				// getopt_long already printed an error message
				break;
			
			default:
				abort();
		}
	}
	
	if (optind == argc) {
		cerr << "Nothing to locate, try --help" << endl;
		exit(1);
	}
	
	term = string(argv[optind++]);
	
	if (optind < argc) {
		cerr << "Too many arguments!" << endl;
		exit(1);
	}
}

// when indexing, case sensitivity is saved so searching can load it
void iconfig::saveSensitivity() {
	ofstream csens(getPath("cs").c_str());
	if (csens.is_open()) {
		csens << caseSensitive << endl;
		csens.close();
	} else {
		perror("Error saving the case sensitivity");
	}
}

void sconfig::loadSensitivity() {
	string line;
	ifstream csens(getPath("cs").c_str());
	if ((csens.is_open()) && (csens.good())) {
		getline(csens,line);
		// csens >> caseSensitive; this is risky
		csens.close();
	} else {
		perror("Error loading the case sensitivity");
	}
	caseSensitive = (line == "1");
	line.clear();
}
