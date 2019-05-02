#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //for bzero()
#include <unistd.h>  //for close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define Server_PortNumber 5555
#define Server_Address "140.120.14.113"

pthread_t thread;
int rc;
struct sockaddr_in address;
int address_length = sizeof(address);
int sock, byte_sent, byte_recv;
char buffer[50];

void *threadFunc(void *arg){

	while(1){
		char buffer1[50];
		byte_recv = recv((intptr_t)sock, buffer1, sizeof(buffer1), 0);

		if (byte_recv < 0){
			printf("Receive Failed\n");
		}
			
		printf("%s\n", buffer1);
	}
	pthread_exit(NULL);
	return 0;
}

int main(int argc, char *argv[])
{
    bzero(&address, address_length);
    address.sin_family = AF_INET;
    address.sin_port = htons(Server_PortNumber);
    address.sin_addr.s_addr = inet_addr(Server_Address);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    
    //printf("This is Client. If you enter '!', The client will be terminate.\n");
    printf("Type message:\n");

    
    if(sock < 0)
        printf("Error creating socket\n");
    //else 	printf("Socket create successfully!\n");

    
	if(connect(sock, (struct sockaddr *)&address, address_length) == -1)
	{
		printf("connect failed!\n");
		close(sock);
	}
	
	rc = pthread_create(&thread, NULL, threadFunc, (void *) (intptr_t) sock);
		
	if(rc){
		printf("Error; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
	while(1){
		bzero(&buffer,sizeof(buffer));
		fgets(buffer, sizeof buffer, stdin);

		if(buffer[0] == '!')
			break;
			
		byte_sent = send(sock, buffer, sizeof(buffer), 0);

		if(byte_sent == -1){
			printf("Error sending packet\n");
			break;
		}	
	}
    close(sock);
    return 0;
}
                                                          
