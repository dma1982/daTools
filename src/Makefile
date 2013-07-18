include ../make.misc

OBJS=log.o \
     fs.o \
     tcp.o \
     event.o \
     buffer.o \
     props.o \
     mem.o \
     md5.o \
     thread.o 

all: libutils.a

CC=g++
INCLUDE=../includes
CFLAGS=${OGL_CFLAGS}

libutils.a: ${OBJS}
	ar cr libutils.a ${OBJS}
	ranlib libutils.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} -I${INCLUDE} $< -o $@

clean:
	rm -f *.o *.a *.orig *~
