# all after symbol '#' is comment
# === which communication library to use ===
CC		=	g++
CFLAGS	=	-Wall -pedantic -g -O0 -ansi
LIBS	=	

all:		compile

compile:	wset eset

wset:		wlocate wdbzer

eset:		elocate edbzer

wdbzer:		set.o file.o word.o wordtree.o config.o db.o wdb.o wdbzer.o
			$(CC) $(CFLAGS) set.o bitmap.o file.o word.o wordtree.o config.o db.o wdb.o wdbzer.o -o wdbzer $(LIBS)

wlocate:	set.o file.o word.o wordtree.o config.o db.o wdb.o wlocate.o
			$(CC) $(CFLAGS) set.o bitmap.o file.o word.o wordtree.o config.o db.o wdb.o wlocate.o -o wlocate $(LIBS)

wdbzer.o:	wdbzer.cpp wdb.o
			$(CC) $(CFLAGS) -c wdbzer.cpp $(LIBS)

wlocate.o:	wlocate.cpp wdb.o
			$(CC) $(CFLAGS) -c wlocate.cpp $(LIBS)

edbzer:		set.o file.o word.o wordtree.o config.o mhregex.o db.o edb.o edbzer.o
			$(CC) $(CFLAGS) set.o bitmap.o file.o word.o wordtree.o config.o mhregex.o db.o edb.o edbzer.o -o edbzer $(LIBS)

elocate:	set.o file.o word.o wordtree.o config.o mhregex.o db.o edb.o elocate.o
			$(CC) $(CFLAGS) set.o bitmap.o file.o word.o wordtree.o config.o mhregex.o db.o edb.o elocate.o -o elocate $(LIBS)

edbzer.o:	edbzer.cpp edb.o
			$(CC) $(CFLAGS) -c edbzer.cpp $(LIBS)

elocate.o:	elocate.cpp edb.o
			$(CC) $(CFLAGS) -c elocate.cpp $(LIBS)

set.o:		set.cpp
			$(CC) $(CFLAGS) -c set.cpp $(LIBS)

bitmap.o:	bitmap.cpp bitmap.hpp
			$(CC) $(CFLAGS) -c bitmap.cpp $(LIBS)

wordtree.o:	wordtree.cpp wordtree.hpp word.o
			$(CC) $(CFLAGS) -c wordtree.cpp $(LIBS)

file.o:		file.cpp file.hpp
			$(CC) $(CFLAGS) -c file.cpp $(LIBS)

word.o:		word.cpp word.hpp bitmap.o
			$(CC) $(CFLAGS) -c word.cpp $(LIBS)

mhregex.o:	mhregex.cpp mhregex.hpp
			$(CC) $(CFLAGS) -c mhregex.cpp $(LIBS)

config.o:	config.cpp config.hpp
			$(CC) $(CFLAGS) -c config.cpp $(LIBS)

db.o:		db.cpp db.hpp set.o config.o file.o wordtree.o
			$(CC) $(CFLAGS) -c db.cpp $(LIBS)

wdb.o:		wdb.cpp wdb.hpp db.o
			$(CC) $(CFLAGS) -c wdb.cpp $(LIBS)

edb.o:		edb.cpp edb.hpp db.o mhregex.o
			$(CC) $(CFLAGS) -c edb.cpp $(LIBS)

clean:
			\rm -rf *.o *.gch wdbzer wlocate edbzer elocate

install:
			\cp -f wdbzer wlocate edbzer elocate /usr/local/bin/

uninstall:
			\rm -f /usr/local/bin/wdbzer /usr/local/bin/wlocate /usr/local/bin/edbzer /usr/local/bin/elocate

# just for my tests
undb:
			\rm -rf env/.wdbaze env/.edbaze

lines:
			\wc -l *.hpp *.cpp

# just for Kessl
run:		undb
			./edbzer -vrenv

