include ../make.misc

CFLAGS=${OGL_CFLAGS}
CC=g++

INCS=-I../src/includes -I../src
LIBS=-L../src/utils/ -L../src/jm -L../src/common -ljm  -lgtest -lACE -lpthread -lconfig++ -lcommon -lutils

BINS=test_buffer test_fs test_tcp_serv test_tcp_client test_log test_mem TestJobOption TestConfig

all: ${BINS}

OBJS=test_log \
     test_buffer \
     test_fs \
     test_tcp_serv \
     test_tcp_client \
     TestJobOption \
     TestConfig \
     test_mem 

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ ${INCS} ${LIBS}

run:
	./TestCreateJobCommand
	./TestJobOption

clean:
	rm -f ${BINS}
