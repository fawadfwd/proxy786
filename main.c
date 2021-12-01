#include "header.h"
void read_n(char *arr,int max)
{
	int i=0;
	while(i<max)
	{
		int valread=read(STDIN_FILENO,arr+i,1);
		if(arr[i]=='\n')
		{arr[i]='\0';break;}

		
		i++;
	}
	//close(sock);



}
void * control_thread(void *status)
{
	int *st=(int*)status;
	char input[100]={0};
	while(1)
	{
		
		printf("Commands:\n1) close:\n2) run:\n\n");
		read_n(input,10);
		if(strcmp(input,PROXY_CLOSE)==0)
		{
			printf("closing\n");
			
			exit(0);
		}
		
	
		
	}
	
	
}
int start_control(int control)
{
	pthread_t thread;
	int  s;   
	pthread_attr_t attr;
	s =  pthread_attr_init(&attr);    
        if (s  != 0) {printf("control/pthread_attr_init\n");exit(EXIT_FAILURE);}

        s =  pthread_attr_setdetachstate(&attr,  PTHREAD_CREATE_DETACHED);	
        if (s  != 0) {printf("control/pthread_attr_setdetachstate\n");exit(EXIT_FAILURE);} 
        
	s=pthread_create(&thread, &attr, control_thread, (void *)&control);
	if (s  != 0) {printf(  "control/pthread_create\n");exit(EXIT_FAILURE);}

	
	return 0;
}
int main()
{

	int state = 1;	
	int server_fd;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int opt = 1;
	int proxy_func=1;//run | 0 = close
	

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		printf("socket failed\n");

		exit(EXIT_FAILURE);
	}
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
			                          &opt, sizeof(opt)))
	{
		printf("setsockopt\n");
		exit(EXIT_FAILURE);
	}
	
	if(setsockopt(server_fd, IPPROTO_TCP, TCP_CORK, &state, sizeof(state)))
	{
		printf("TCP CORK\n");
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	if (bind(server_fd, (struct sockaddr *)&address,
		                 sizeof(address))<0)
	{
		printf("bind failed\n");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		printf("listen\n");
		exit(EXIT_FAILURE);
	}
	
	int ret=start_control(proxy_func);
	accept1(server_fd,address,addrlen);
	return 0;
}



