include ../make.misc

CFLAGS=${OGL_CFLAGS}
CC=g++

INCS=-I../src/includes -I../src
LIBS=-L../src/utils/ -L../src/jm -L../src/common -ljm  -lgtest -lACE -lpthread -lconfig++ -lcommon -lutils

BINS=test_buffer test_fs test_tcp_serv test_tcp_client test_log test_mem TestJobOption TestConfig TestTaskOption

all: ${BINS}

OBJS=test_log \
     test_buffer \
     test_fs \
     test_tcp_serv \
     test_tcp_client \
     TestJobOption \
     TestTaskOption \
     TestConfig \
     test_mem 

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ ${INCS} ${LIBS}

clean:
	rm -f ${BINS}
