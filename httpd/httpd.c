#include"httpd.h"

void usage(const char *proc) 
{
	printf("usage httpd: %s [port]\n", proc);
}

static void print_debug(const char *msg)
{
#ifdef _DEBUG_ 
	printf("%s\n", msg);
#endif
}

static void bad_request(int client)
{
	print_debug("enter out fault...");
	char buf[1024];
	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<html></br><p>your enter message is a bad request</p></br><html>\r\n");
	send(client, buf, sizeof(buf), 0);
}

static void not_found(int client)
{
	print_debug("enter out fault...");
	char buf[1024];
	sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<html></br><p>the resource you have requested has not found</p></br><html>\r\n");
	send(client, buf, sizeof(buf), 0);
}

static void server_error(int client)
{
	print_debug("enter out fault...");
	char buf[1024];
	sprintf(buf, "HTTP/1.0 500 _SERVER_ERROR_\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<html></br><p>server has something wrong, please request later</p></br><html>\r\n");
	send(client, buf, sizeof(buf), 0);
}

void print_log(int line, int err, const char *err_str)
{
	printf("[%d] [%d] [%s]\n", line, err, err_str);
}

int get_line(int sock, char *buf, int max_len)
{
	if( !buf || max_len < 0)
		return -1;
	int i = 0;
	int n = 0;
	char c = '\0';
	while(i < max_len-1 && c != '\n')
	{
		n = recv(sock, &c, 1, 0);
		if(n > 0) //success
		{
			if(c == '\r')
			{
				
				n = recv(sock, &c, 1, MSG_PEEK);
				if(n>0 && c=='\n')  //windows \r\n  -> \n
					recv(sock, &c, 1, 0);	//delete
				else
					c = '\n';  //Mac os \r  -> \n
			}
			buf[i++] = c;
		}
		else
		{
			c = '\n';
		}
	}
	buf[i] = '\0';
	return i;
}

void echo_error_to_client(int client, int error_code)
{
	switch(error_code)
	{
		case 400:	//request error
			bad_request(client);
			break;
		case 404:	//not found
			not_found(client);
			break;
		case 500:	//server error
			server_error(client);
			break;
		case 503:   //server unavailable
			server_error(client);
			break;
		default:
			break;
	}
}

void echo_html(int client, const char *path, unsigned int file_size)
{
	if( !path)
		return ;
	int	in_fd = open(path, O_RDONLY);
	if(in_fd < 0)  //open error
	{
		print_debug("open index.html error");
		return ;
	}
	//open success
	print_debug("open index.html success");
	char echo_line[1024];
	bzero(echo_line, sizeof(echo_line));
	strncpy(echo_line, HTTP_VERSION, 12+1);  //strlen(HTTP_VERSION)
	strcat(echo_line, " 200 OK");
	strcat(echo_line, "\r\n\r\n");

	send(client, echo_line, strlen(echo_line), 0); //
	print_debug("send echo head success");
	if(sendfile(client, in_fd, NULL, file_size) < 0) //more effect
	{
		print_debug("send_file error");
		close(in_fd);
		return ;
	}
	print_debug("sendfile success");
	close(in_fd);
}


void clear_header(int client)
{
	char buf[1024];
	bzero(buf, sizeof(buf));
	int ret = 0;
	do
	{	
		ret = get_line(client, buf, sizeof(buf));
	}while(ret>0 && strcmp(buf, "\n")!=0);
}

void exe_cgi(int sock_client, const char *path, const char *method, const char *query_string)
{
	print_debug("enter exe_cgi...");

	char buf[_COMM_SIZE_];
	int numchars = 0;
	int content_length = -1;

	int cgi_input[2] = {0, 0};   //input pipe
	int cgi_output[2] = {0, 0};  //output pipe
	pid_t id;
	
	if(strcasecmp(method, "GET") == 0) //GET
	{
		clear_header(sock_client);
	}
	else  //POST
	{
		do
		{
			bzero(buf, sizeof(buf));
			numchars = get_line(sock_client, buf, sizeof(buf));
			//get Content-Length
			if(strncasecmp(buf, "Content-Length:", strlen("Content_Length:")) == 0)
			{
				content_length = atoi(&buf[16]);
			}
		}while(numchars > 0 && strcmp(buf, "\n") != 0);

		if(content_length == -1)
		{
			return ;
		}
	}
	printf("content_length : %d\n", content_length);

	bzero(buf, sizeof(buf));
	strcpy(buf, HTTP_VERSION);
	strcat(buf, " 200 OK\r\n\r\n");
	//send the response line to client
	send(sock_client, buf, strlen(buf), 0);   //eg: HTTP/1.0 200 OK
	
	if(pipe(cgi_input) == -1)
	{
		echo_error_to_client(sock_client, _SERVER_ERROR_);
		return ;
	}
	if(pipe(cgi_output) == -1)
	{
		close(cgi_input[0]);
		close(cgi_input[1]);
		echo_error_to_client(sock_client, _SERVER_ERROR_);
		return ;
	}
	if( (id = fork()) < 0)
	{
		close(cgi_input[0]);
		close(cgi_input[1]);
		close(cgi_output[0]);
		close(cgi_output[1]);
		echo_error_to_client(sock_client, _SERVER_ERROR_);
		return ;
	}
	if(id == 0) //child
	{
		char query_env[_COMM_SIZE_/10];	    //store query_string and putenv
		char method_env[_COMM_SIZE_];       //store request method and putenv
		char content_len_env[_COMM_SIZE_];  //store content_length and putenv
		bzero(query_env,sizeof(query_env));
		bzero(method_env, sizeof(method_env));
		bzero(content_len_env, sizeof(content_len_env));

		close(cgi_input[1]);
		close(cgi_output[0]);
		dup2(cgi_input[0], 0);   //cgi_input[0] -> stdin
		dup2(cgi_output[1], 1);  //cgi_output[1] -> stdout

		sprintf(method_env, "REQUEST_METHOD=%s", method);
		putenv(method_env);

		if(strcasecmp("GET", method) == 0) //GET
		{
			sprintf(query_env, "QUERY_STRING=%s", query_string);
			putenv(query_env);
		}
		else  //POST
		{
			sprintf(content_len_env, "CONTENT_LENGTH=%d", content_length);
			printf("content_len_env : %s", content_len_env);
			putenv(content_len_env);
		}
		execl(path, path, NULL);   //child to run the cgi program and exit
		exit(1);
	}
	else  //father
	{
		close(cgi_input[0]);
		close(cgi_output[1]);
		int i = 0;
		char c = '\0';
		if(strcasecmp("POST", method) == 0)
		{
			for(; i< content_length; ++i)
			{
				recv(sock_client, &c, 1, 0); //get the data from client
				write(cgi_input[1], &c, 1);  //put the data to child
			}
		}
		while(read(cgi_output[0], &c, 1) > 0)
		{
			send(sock_client, &c, 1, 0); //get the data from child and put it to client
		}
		close(cgi_input[1]);
		close(cgi_output[0]);

		waitpid(id, NULL, 0);   //wait the child who run cgi program
	}
	printf("off exe_cgi...\n");
}


void *accept_request(void *arg)
{
	printf("get a new connect request...\n");
	//detach the thread, the father well not wait the child
	pthread_detach(pthread_self());	

	//get the sockfd connected to client
	int sock_client = *(int *)arg;
	int cgi = 0;           //if cgi == 1, exe_cgi

	char path[_COMM_SIZE_];			//store the path of resource
	char method[_COMM_SIZE_/10];    //store the request method
	char url[_COMM_SIZE_];          //stort the url
	char buffer[_COMM_SIZE_];
	char *query_string = NULL;      //store the args in GET
	
	bzero(path, sizeof(path));
	bzero(method, sizeof(method));
	bzero(url, sizeof(url));
	bzero(buffer, sizeof(buffer));

	if(get_line(sock_client, buffer, sizeof(buffer)) < 0) //connect sucess,  buffer == GET / HTTP/1.0
	{
		echo_error_to_client(sock_client, _SERVER_ERROR_);
		return NULL;
	}
	
	int i = 0; 
	int j = 0;  //buffer line index
	while(!isspace(buffer[j]) && i<sizeof(method)-1 && j<sizeof(buffer))  //buffer[j] == ' ' -> return 0;
	{
		method[i] = buffer[j];
		++i;
		++j;
	}

	if(strcasecmp(method, "GET") && strcasecmp(method, "POST"))
	{
		return NULL;
	}

	//clear space, point to useful url start
	while(isspace(buffer[j]) && j<sizeof(buffer))
	{
		j++;
	}

	//get url
	i = 0; 
	while(!isspace(buffer[j]) && i<sizeof(url)-1 && j<sizeof(buffer))
	{

		url[i] = buffer[j];
		i++;
		j++;	
	}
	
	print_debug(method);
	print_debug(url);
	
	if(strcasecmp(method, "POST") == 0)
		cgi = 1;
	
	if(strcasecmp(method, "GET") == 0)
	{
		query_string = url;
		while(*query_string != '?' && *query_string != '\0')
			query_string++;
		//get query_string
		if(*query_string == '?')  //url == /xxx/xx?arg
		{
			*query_string = '\0';	
			query_string++;
			cgi = 1;
		}
	}

	sprintf(path, "htdocs%s", url); //htdocs: work path

	if(path[strlen(path)-1] == '/')
	{
		strcat(path, MAIN_PAGE);
	}

	print_debug(path);	

	struct stat st;
	if(stat(path, &st) < 0)  //failed, file does not exist
	{
		print_debug("miss cgi");
		clear_header(sock_client);
		echo_error_to_client(sock_client, _REQUEST_ERROR_);
	}
	else	//file exist
	{
		if(S_ISDIR(st.st_mode))
		{
			strcat(path, "/");
			strcat(path, MAIN_PAGE);
		}
		else if(st.st_mode & S_IXUSR ||
				st.st_mode & S_IXGRP ||
				st.st_mode & S_IXOTH)   //file has execu 
		{
			cgi = 1;
		}
		else
		{}

		if(cgi) //dynamic html, need to run cgi;
			exe_cgi(sock_client, path, method, query_string);
		else   //static html;
		{
			clear_header(sock_client);
			echo_html(sock_client, path, st.st_size);
		}
	}
	printf("close sock...\n");
	close(sock_client);

	return NULL;
}


int main(int argc, char *argv[])
{
	Threadpool pool;

	if(argc != 2)
	{
		usage(argv[0]);
		exit(1);
	}

	struct sockaddr_in servaddr, clieaddr;
	socklen_t len;
	struct epoll_event tmp, saveReady[/*OPEN_MAX*/1024];
	int listenfd, connfd, sockfd, epfd;
	int maxindex, ready, i, j,  num;
	char buf[/*BUF_SIZE*/1024], str[INET_ADDRSTRLEN];	//INET_ADDRSTRLEN = 16

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("socket");
		exit(1);
	}

	if(listen(listenfd, 10) < 0)
	{
		perror("socket");
		exit(1);
	}

	if((epfd = epoll_create(/*OPEN_MAX*/1024)) < 0)
	{
		perror("epoll_create");
		exit(1);
	}

	tmp.events = EPOLLIN;
	tmp.data.fd = listenfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &tmp);

	while(1)
	{
		ready = epoll_wait(epfd, saveReady, /*OPEN_MAX*/1024, -1);

		if(ready < 0)    // * epoll_wait 出错
		{	
			perror("epoll_wait");
			exit(1);
		}
		else if(ready > 0)
		{
			for(i = 0; i < ready; ++i)
			{
				if(saveReady[i].data.fd == listenfd)		//有新客户端连接
				{
					len = sizeof(clieaddr);
					if((connfd = accept(listenfd, (struct sockaddr *)&clieaddr, &len)) < 0)    // * 与客户端建立连接
					{
						perror("accept");
						exit(1);
					}
#ifdef __DEBUG__
					//打印客户端信息
					//printf("get a new connecter...%d\n", numofrequest++);
					const char *client_ip = inet_ntop(AF_INET, &clieaddr.sin_addr, str, sizeof(str));
					short client_port = ntohs(clieaddr.sin_port);
					printf("get a new client at   ip: %s,  port: %d\n", client_ip, client_port); 
#endif

					// * 把关联了客户端的文件描述符 加入到内核事件表里面					
					tmp.events = EPOLLIN;
					tmp.data.fd = connfd;
					epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &tmp);		//把accept添加到要监控的epfd里面
				}
				else    // * 客户有请求
				{
					// 1 迭代型服务
					//accept_request((void *)connfd);
					// * 多线程版本
					//a new thread to handle a connectiong request
					
					// 2 多线程版本
					//pthread_t new_thread;
					//pthread_create(&new_thread, NULL, accept_request, (void *)connfd);

					// 3 线程池版本
					pthread_t new_thread;
					int newConnfd = connfd;
					pool.AddWorkToWorkQueue(accept_request, (void *)&newConnfd);    // * 每接收一个连接文件描述符，就把该事件喂给线程池
					
					
					// ****** 非常非常重要，如果不把connfd从事件表中删除掉，下一次epoll_wait还会进入这个里面
					epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);     
				}
			}
		}
	}

	close(listenfd);
	close(epfd);

	return 0;	
}

// * sucess return sock, else exit process
//int start(int port) 
//{ 
//	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);  
//	if(listen_sock == -1) 
//	{ 
//		print_log(__LINE__, errno, strerror(errno));
//		exit(1);
//	}
//
//	struct sockaddr_in local;  
//	local.sin_family = AF_INET;
//	local.sin_port = htons(port);  //host -> net
//	local.sin_addr.s_addr = htonl(INADDR_ANY);
//
//	socklen_t len = sizeof(local);
//	
//	//reuse port
//	int flag = 1;
//	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag));
//
//	if(bind(listen_sock, (struct sockaddr *)&local, len) == -1)   // 2 bind
//	{
//		print_log(__LINE__, errno, strerror(errno));
//		exit(2);
//	}
//
//	if(listen(listen_sock, _BACK_L0G_) == -1)
//	{
//		print_log(__LINE__, errno, strerror(errno));
//		exit(3);	
//	}
//
//	return listen_sock;
//}


//int main(int argc, char *argv[])
//{
//	if(argc != 2)
//	{
//		usage(argv[0]);
//		exit(1);
//	}
//	int port = atoi(argv[1]);
//	
//	//get the listen socket
//	int sock = start(port); 
//
//	//store the infomation of client
//	struct sockaddr_in client;
//	socklen_t len = 0;
//	while(1)
//	{
//		//wait for connection
//		int new_sock = accept(sock, (struct sockaddr *)&client, &len);
//		if(new_sock < 0)
//		{
//			print_log(__LINE__, errno, strerror(errno));
//			continue;
//		}
//		pthread_t new_thread;
//		//a new thread to handle a connectiong request
//		pthread_create(&new_thread, NULL, accept_request, (void *)new_sock);
//	}
//
//	return 0;
//}
