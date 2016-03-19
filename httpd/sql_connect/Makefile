WORK_PATH=$(shell pwd)

BIN=sql_connect
SRC=sql_connect.cpp

INCLUDE=-I${WORK_PATH}/mysql_lib/usr/include/mysql -I.
LIB_PATH=-L${WORK_PATH}/mysql_lib/usr/lib64/mysql
LIB_NAME=-lmysqlclient
LDFLAGS=-ldl -lpthread -lrt

CC=g++
FLAGS=-g -o 
${BIN}:${SRC}
	${CC} ${FLAGS} $@ $^ ${INCLUDE} ${LIB_PATH} $(LIB_NAME) $(LDFLAGS)

.PHONY:clean
clean:
	rm -rf ${BIN}
