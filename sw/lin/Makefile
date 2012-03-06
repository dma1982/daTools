OBJS=log.o \
     fs.o \
     tcp.o \
     event.o \
     buffer.o \
     conf.o

all: libsw.a

CC=g++
INCLUDE=../includes
CFLAGS=-Wall -DLIN

libsw.a: ${OBJS}
	ar cr libsw.a ${OBJS}
	ranlib libsw.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} -I${INCLUDE} $< -o $@

clean:
	rm -f *.o *.a *.orig *~
