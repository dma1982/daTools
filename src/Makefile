include ../make.misc

OBJS=log.o \
     fs.o \
     tcp.o \
     event.o \
     buffer.o \
     props.o \
     mem.o \
     md5.o \
     hash.o \
     thread.o 

all: libogl.a

CC=g++
INCLUDE=../includes
CFLAGS=${OGL_CFLAGS}

libogl.a: ${OBJS}
	ar cr libogl.a ${OBJS}
	ranlib libogl.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} -I${INCLUDE} $< -o $@

clean:
	rm -f *.o *.a *.orig *~
