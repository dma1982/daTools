include ../make.misc

CFLAGS=${OGL_CFLAGS}
CC=g++

INCS=-I../src/includes -I../src
LIBS=-L../src/utils/ -L../src/jm -ljm -lutils -lgtest -lACE -lpthread

BINS=test_buffer test_fs test_tcp_serv test_tcp_client test_log test_mem TestCreateJobCommand TestJobOption

all: ${BINS}

OBJS=test_log \
     test_buffer \
     test_fs \
     test_tcp_serv \
     test_tcp_client \
     TestCreateJobCommand \
     TestJobOption \
     test_mem 

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ ${INCS} ${LIBS}

run:
	./TestCreateJobCommand
	./TestJobOption

clean:
	rm -f ${BINS}
