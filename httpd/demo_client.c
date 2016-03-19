#include"demo_client.h"

void usage(const char *argv)
{
	printf("usage : %s [IP] [PORT] [CMD]\n", argv);
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		usage(argv[0]);
		exit(1);
	}
	//get the listen socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("sock");
		exit(1);
	}	
	int port = atoi(argv[2]);

	struct sockaddr_in remote_server;
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons(port);
	remote_server.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sock, (struct sockaddr *)&remote_server, sizeof(remote_server)))
	{
		perror("connect");
		exit(2);
	}	

	const char *cmd = argv[3];
	int len = strlen(cmd);
	ssize_t size = send(sock, cmd, len, 0);
	if(size <= 0)
	{
		perror("send");
		close(sock);
		return 1;
	}

	char buf[1024];
	memset(buf, '\0', sizeof(buf));
	while(recv(sock, buf, sizeof(buf)-1, 0) > 0)
	{
		printf("%s", buf);
		memset(buf, '\0', sizeof(buf));
	}

	close(sock);
	return 0;
}
