#ifndef _HTTPD_H__
#define _HTTPD_H__

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <./asm-generic/socket.h>


#include "threadpool.h"

#define _BACK_L0G_ 5
#define _COMM_SIZE_ 1024
#define MAIN_PAGE "index.html"
#define HTTP_VERSION "HTTP/1.0"

#define _REQUEST_ERROR_ 400
#define _SERVER_ERROR_ 500
#define _NOT_FOUNT_ 404


//#define __DEBUG__

#endif

