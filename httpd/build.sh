#!/bin/bash

make clean
make
make output
export LD_LIBRARY_PATH=/home/common/YP/httpd/sql_connect/mysql_lib/lib #common
