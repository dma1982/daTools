bin:
	cd src; make

ogl: bin
	mkdir output -p
	mkdir output/bin -p
	mkdir output/etc -p
	mkdir output/conf -p
	cp src/ogl.conf output/conf
	cp src/jm/jmd output/etc/
	cp src/jr/jrd output/etc/
	cp src/cli/oglsub output/bin/
	cd output; tar cvf reeline_${OGL_VER}.tar bin etc conf ; gzip reeline_${OGL_VER}.tar

all:
	cd src; make
	cd test; make

clean:
	cd src; make clean
	cd test; make clean
	rm -rf output

astyle:
	astyle `find . -name *.cpp -o -name *.h`
	rm -f `find . -name *.orig | xargs`
