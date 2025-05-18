all: main
	mkdir -p build/bin
	mkdir -p build/lib
	mkdir -p build/objects
	mv *.o build/objects
	mv *.a build/lib
	mv main build/bin

main: main.o testCase.o liblinkedlist.a
	gcc main.o testCase.o -o main -llinkedlist -L.

test: test.o liblinkedlist.a
	mkdir -p build/bin
	mkdir -p build/lib
	mkdir -p build/objects
	gcc test.o -o build/bin/test -llinkedlist -L.
	mv *.o build/objects
	mv *.a build/lib

test.o: test/test.c
	gcc test/test.c -c -I include

main.o: src/bin/main.c
	gcc src/bin/main.c -c -I include

testCase.o: src/bin/testCase.c
	gcc src/bin/testCase.c -c -I include

liblinkedlist.a: src/lib.c
	gcc src/lib.c -c -I include
	ar rs liblinkedlist.a lib.o

clean:
	rm -rf build
