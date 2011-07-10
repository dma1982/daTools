SW_LIB=libsw.a
CC=g++
OS_TYPE=lin
CFLAGS=-c -Wall -DLIN
SOURCES= sw/fs.cpp  sw/process.cpp  sw/tcp.${OS_TYPE}.cpp
OBJECTS=${SOURCES:.cpp=.o}

all: $(SOURCES) $(SW_LIB)

${SW_LIB}: ${OBJECTS}
	ar -r $@ ${SW_LIB}
	ranlib ${SW_LIB}

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -f sw/*.o 
	rm -f ${SW_LIB}

