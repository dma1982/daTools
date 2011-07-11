include make.misc


CFLAGS=-Wall -O2 ${SW_CFLAGS}
CC=g++


all: test_buffer test_util test_fs test_tcp_serv test_tcp_client

test_buffer: test_buffer.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE}  $< -o $@

test_util: test_util.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE}  $< -o $@

test_fs: test_fs.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE} ${SW_FS_SRC} $< -o $@

test_tcp_serv: test_tcp_serv.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE} ${SW_NET_SRC} $< -o $@

test_tcp_client: test_tcp_client.cpp
	${CC} ${CFLAGS} -I${SW_INCLUDE} ${SW_NET_SRC} $< -o $@



clean:
	rm -f test_buffer test_util test_fs test_tcp_serv test_tcp_client
