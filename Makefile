bin:
	cd src; make

ogl: bin
	rm -rf output
	mkdir output -p
	mkdir output/bin -p
	mkdir output/etc -p
	mkdir output/conf -p
	mkdir output/lib64 -p
	mkdir output/logs -P
	cp -R conf output/
	cp -R lib64 output/
	cp ogl.bashrc output/
	cp src/jm/jmd output/etc/
	cp src/jr/jrd output/etc/
	cp src/cli/oglsub output/bin/
	cd output; tar cvf reeline_${OGL_VER}.tar logs lib64 ogl.bashrc bin etc conf ; gzip reeline_${OGL_VER}.tar

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
