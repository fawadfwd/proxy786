#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define PORT 5002

#define PROXY_RUN "run"

#define PROXY_CLOSE "close"

#define IN_REQUEST_SIZE 2048

struct fd_manager
{
	int in_fd;
	int out_fd;
	char *in_request_buf;
	char *out_request_buf;
	char *response;
};

int accept1(int server_fd,struct sockaddr_in address,int addrlen);
int in_read(struct fd_manager *con);
