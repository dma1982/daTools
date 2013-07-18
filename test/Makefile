include ../make.misc

CFLAGS=${OGL_CFLAGS}
CC=g++

INCS=-I../includes -I../src
LIBS=-L../src/ -lgtest -lgtest_main -lpthread -logl

BINS=TestHash

all: ${BINS}

OBJS=TestHash

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ ${INCS} ${LIBS}

clean:
	rm -f ${BINS}
