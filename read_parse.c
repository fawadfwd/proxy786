#include "header.h"


#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

int strncmp_n(char *a,char *b,int size)
{
	int x=0;
	while(x<size)
	{
		//printf("%c %c %d\n",a[x],b[x], (a[x]==b[x]));
		if(a[x]!=b[x])
		
		{
			
			return -1;
		}
		//printf("%d\n",x);
		x++;
	}
	return 0;
}
int get_host(char *in_request_buf,int val,char **host)
{
	int i=0;
	
	while(i<val)
	{
		//printf("%s\n\n",(in_request_buf+i));
	
		int x=strncmp_n((in_request_buf+i),"Host:",strlen("Host:"));

		if(x==0)
		{
			in_request_buf=in_request_buf+sizeof("Host:");
			char *temp=strtok(&in_request_buf[i],"\n");
			*host=malloc(strlen(temp)+1);
			if(*host==NULL)
			{
				printf("heap problem \n");
				//exit(EXIT_FAILURE);
			}
			strcpy(*host,strtok(&in_request_buf[i],"\n"));
			//printf("**************************************\n");
		
			int k=0;
			//printf("666 [%s]\n",*host);
		 	while(host[0][k]!='\r')
		 	{
		 		k++;
		 	}
		 	host[0][k]='\0';
			return k;
			
			
		}
		i++;
	}

	return 0;
}
int get_ssl_response(char *host,char **response,int fd)
{
	BIO* bio;
	SSL* ssl;
	SSL_CTX* ctx;

	//
	//   Registers the SSL/TLS ciphers and digests.
	//
	//   Basically start the security layer.
	//
	SSL_library_init();

	//
	//  Creates a new SSL_CTX object as a framework to establish TLS/SSL
	//  or DTLS enabled connections
	//
	ctx = SSL_CTX_new(SSLv23_client_method());

	//
	//  -> Error check
	//
	if (ctx == NULL)
	{
		printf("Ctx is null\n");
	}

	//
	//   Creates a new BIO chain consisting of an SSL BIO
	//
	bio = BIO_new_ssl_connect(ctx);

	//
	//  Use the variable from the beginning of the file to create a 
	//  string that contains the URL to the site that you want to connect
	//  to while also specifying the port.
	//
	BIO_set_conn_hostname(bio, host);

	//
	//   Attempts to connect the supplied BIO
	//
	if(BIO_do_connect(bio) <= 0)
	{
		printf("Failed connection\n");
		return 1;
	}
	else
	{
	printf("Connected\n");
	}

	//
	//  The bare minimum to make a HTTP request.
	//
	char* write_buf = "GET / HTTP/1.1\r\n"
	"Host: www.google.com:443\r\n"
	"Connection: close\r\n"
	"\r\n";

	//
	//   Attempts to write len bytes from buf to BIO
	//
	if(BIO_write(bio, write_buf, strlen(write_buf)) <= 0)
	{
	//
	//  Handle failed writes here
	//
	if(!BIO_should_retry(bio))
	{
	// Not worth implementing, but worth knowing.
	}

	//
	//  -> Let us know about the failed writes
	//
	printf("Failed write\n");
	}

	//
	//  Variables used to read the response from the server
	//
	int size;
	char buf[1024];

	//
	//  Read the response message
	//
	for(;;)
	{
		//
		//  Get chunks of the response 1023 at the time.
		//
		size = BIO_read(bio, buf, 1023);

		//
		//  If no more data, then exit the loop
		//

		if(size <= 0)
		{
			break;
		}

		//
		//  Terminate the string with a 0, to let know C when the string 
		//  ends.
		//
		buf[size] = 0;
		int ret=write(fd,buf,size);
		if(ret!=-1)
		printf("written %d characters\n",ret);
		//
		//  ->  Print out the response
		//
		//printf("%s", buf);
	}
	sleep(5);
	//
	//  Clean after ourselves
	//
	//BIO_free_all(bio);
	//SSL_CTX_free(ctx);


	return 0;
}

int in_read(struct fd_manager *con)
{
	con->in_request_buf=malloc(sizeof(char)*IN_REQUEST_SIZE); //2048 character long of in request from browser GET + POST + etc
	if(con->in_request_buf==NULL)
	{
		printf("in_read/heap error \n");
		//exit(EXIT_FAILURE);
	}
	//con->in_request_buf[IN_REQUEST_SIZE-1]='\0';//0 ?
	//printf("strlen(con->in_request_buf) = %zu\n",strlen(con->in_request_buf));
	int val=0;
	int total=0;
	while(total<IN_REQUEST_SIZE)
	{
	
		val=read(con->in_fd,&(con->in_request_buf)[total],(IN_REQUEST_SIZE-total));
		
		total=total+val;
		if(val>0)//received request data
		{
			
			//printf("data in buf \n%s\n",con->in_request_buf);
			//printf("\nlen = %zu\n",strlen(con->in_request_buf));
			char *host=NULL;//will get null-terminated string
			//printf("*******%d\n",val);
			
			int ret = get_host(con->in_request_buf,total,&host);

			if(ret>0)
			{
				char **response=NULL;
				get_ssl_response(host,response,con->in_fd);
				printf("%s\n",host);
				sleep(5);
				//get port;
				//openssl for fetch
				//then  write the openssl real to in_fd
			}
			//printf("*******wola = [%s]*********\n",host);		        

						
		}
		else if(val==0)//remote close
		{
			
			//close(con->in_fd);
			if(con!=NULL)
			{
				//free(con->in_request_buf);
				//free(con);
				//con=NULL;
			}
			return -1;
		}
		else//some error
		{
			
			//close(con->in_fd);
			if(con!=NULL)
			{
				//free(con->in_request_buf);
				//free(con);
				//con=NULL;
			}
			return -1;
		
		}
	} 	
	
	
	
	return total;
}
