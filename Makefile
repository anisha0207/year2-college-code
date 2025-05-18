test: libpack109.a test.o
	g++ build/objects/release/test.o -o tester -lpack109 -Lbuild/lib/release -std=c++11
	mkdir -p build/bin/release
	mv tester build/bin/release/test

libpack109.a:
	g++ src/lib.cpp -c -Iinclude -std=c++11
	ar rs libpack109.a lib.o 
	mkdir -p build/lib/release
	mkdir -p build/objects/release
	mv *.o build/objects/release
	mv libpack109.a build/lib/release

test.o:
	g++ test/test.cpp -c -lpack109 -Lbuild/lib/release -Iinclude -std=c++11
	mkdir -p build/objects/release
	mv test.o build/objects/release

clean:
	rm -f *.a
	rm -f *.o
	rm -rf build