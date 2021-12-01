#include "header.h"

void * handle_in(void *arg)
{
	struct fd_manager *con=(struct fd_manager *)arg;
	//printf("handle in fd = %d\n",con->in_fd);
	in_read(con);

	//close(con->in_fd);
	//free(con);
	return (void *)0;
}

int accept1(int server_fd,struct sockaddr_in address,int addrlen)
{
	while(1)
	{
		int new_socket;
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
		(socklen_t*)&addrlen))<0)
		{
			printf("accept\n");
			//exit(EXIT_FAILURE);
		}
		pthread_t thread_get;
		int  s;   
		pthread_attr_t attr;
		
		struct fd_manager *con=malloc(sizeof(struct fd_manager));
		
		if(con==NULL)
		{
			printf("HEAP error -- malloc call\n");
			
		}
		
		con->in_fd=new_socket;
		//printf("new_socket = %d\n",new_socket);
		s =  pthread_attr_init(&attr);    
 	        if (s  != 0) {printf("pthread_attr_init\n");exit(EXIT_FAILURE);}
 	        
 	        s =  pthread_attr_setdetachstate(&attr,  PTHREAD_CREATE_DETACHED);	
	        if (s  != 0) {printf("pthread_attr_setdetachstate\n");exit(EXIT_FAILURE);}
	        s=pthread_create(&thread_get, &attr, handle_in, (void *)con);
		if (s  != 0) {printf(  "pthread_create\n");exit(EXIT_FAILURE);}
		 


		
		
		
		
	
	}
	
	return 0;
}
