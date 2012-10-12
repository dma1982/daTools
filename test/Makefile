include ../make.misc

CFLAGS=${OGL_CFLAGS}
CC=g++

all: test_buffer test_fs test_tcp_serv test_tcp_client test_log test_mem 

OBJS=test_log \
     test_buffer \
     test_fs \
     test_tcp_serv \
     test_tcp_client \
     test_mem 

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ -lpthread -L../src/utils/ -lutils -I../src/includes

clean:
	rm -f test_buffer test_log test_fs test_tcp_serv test_tcp_client test_mem 
