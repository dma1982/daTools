include make.misc

CFLAGS=${SW_CFLAGS}
CC=g++


all: test_buffer test_fs test_tcp_serv test_tcp_client test_log test_mem

OBJS=test_log \
     test_buffer \
     test_fs \
     test_tcp_serv \
     test_tcp_client \
     test_mem

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ -lpthread -L${SW_LIB} -lutils -I${SW_INCLUDE} 

clean:
	rm -f test_buffer test_log test_fs test_tcp_serv test_tcp_client test_mem
