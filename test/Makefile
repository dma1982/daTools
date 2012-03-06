include make.misc

CFLAGS= -O2 ${SW_CFLAGS}
CC=g++


#all: test_buffer test_util test_fs test_tcp_serv test_tcp_client
all: test_buffer test_fs test_tcp_serv test_tcp_client

test_buffer: test_buffer.cpp
	${CC} ${CFLAGS} -o $@ $< -lpthread -L${SW_LIB} -lsw -I${SW_INCLUDE}

test_util: test_util.cpp
	${CC} ${CFLAGS} -o $@ $< -lpthread -L${SW_LIB} -lsw -I${SW_INCLUDE}

test_fs: test_fs.cpp
	${CC} ${CFLAGS} -o $@ $< -lpthread -L${SW_LIB} -lsw -I${SW_INCLUDE}

test_tcp_serv: test_tcp_serv.cpp
	${CC} ${CFLAGS} -o $@ $< -lpthread -L${SW_LIB} -lsw -I${SW_INCLUDE}

test_tcp_client: test_tcp_client.cpp
	${CC} ${CFLAGS} -o $@ $< -lpthread -L${SW_LIB} -lsw -I${SW_INCLUDE}



clean:
	rm -f test_buffer test_util test_fs test_tcp_serv test_tcp_client
