include make.misc

CFLAGS=-O2 ${SW_CFLAGS}
CC=g++


#all: test_buffer test_util test_fs test_tcp_serv test_tcp_client
all: test_buffer

test_buffer: test_buffer.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE} ${SW_SRC}  $< -o $@

test_util: test_util.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE} ${SW_SRC} $< -o $@

test_fs: test_fs.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE} ${SW_SRC} $< -o $@

test_tcp_serv: test_tcp_serv.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE} ${SW_SRC} $< -o $@

test_tcp_client: test_tcp_client.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE} ${SW_SRC} $< -o $@



clean:
	rm -f test_buffer test_util test_fs test_tcp_serv test_tcp_client
