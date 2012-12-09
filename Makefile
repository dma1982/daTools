bin:
	cd src; make

all:
	cd src; make
	cd test; make

clean:
	cd src; make clean
	cd test; make clean

astyle:
	astyle `find . -name *.cpp -o -name *.h`
	rm -f `find . -name *.orig | xargs`
