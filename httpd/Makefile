PWD=$(shell pwd)

CGI_PATH=$(PWD)/cgi_bin

SER_BIN=httpd
CLI_BIN=demo_client
SER_SRC=httpd.c
CLI_SRC=demo_client.c

INCLUDE=.
CC=gcc
FLAGS=-o 
LDFLAGS=-lpthread#-static
LIB=

.PHONY:all
all:$(SER_BIN) $(CLI_BIN) cgi

$(SER_BIN):$(SER_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS) -g -D_DEBUG_
$(CLI_BIN):$(CLI_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS) -g -D_DEBUG_

.PHONY:cgi
cgi:
	@for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
		make;\
		cd -;\
	done


.PHONY:output
output: all
	#export LD_LIBRARY_PATH=/home/common/YP/httpd/sql_connect/mysql_lib/lib #common
	@mkdir -p output/cgi_bin
	@cp httpd output
	@cp demo_client output
	@cp -rf conf output
	@cp -rf log output
	#@cp start.sh output
	@cp -rf htdocs output
	@mkdir -p output/htdocs/cgi_bin
	#@cp -rf htdocs/* output/htdocs/cgi_bin  #common
	@for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
		make output;\
		cd -;\
	done


.PHONY:clean
clean:
	@rm -rf $(SER_BIN) $(CLI_BIN) output
	@for name in `echo #(CGI_PATH)`;\
	do\
		cd $$name;\
		make clean;\
		cd -;\
	done
