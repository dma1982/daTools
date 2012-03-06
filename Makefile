all:
	cd sw; make
	cd test; make

clean:
	cd sw; make clean
	cd test; make clean
