include ../make.misc

CFLAGS=${OGL_CFLAGS}
CC=g++

INCS=-I../includes -I../src
LIBS=-L../src/ -lgtest -lpthread -lutils

BINS=test_buffer test_fs test_tcp_serv test_tcp_client test_log test_mem 

all: ${BINS}

OBJS=test_log \
     test_buffer \
     test_fs \
     test_tcp_serv \
     test_tcp_client \
     test_mem 

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ ${INCS} ${LIBS}

clean:
	rm -f ${BINS}
