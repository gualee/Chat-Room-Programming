#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#define PortNumber 5555
#define MaxClientSize 20
int clients[MaxClientSize] = {0};

pthread_t thread;
int rc, count = 0;
struct sockaddr_in server_addr, client_addr;
int sock, byte_recv, byte_sent, recfd;
char buffer[50];
unsigned int server_addr_length = sizeof(server_addr), client_addr_length = sizeof(client_addr);
int client_id[MaxClientSize]={0};
int c_count=0;

void *threadFunc(void *arg){
	int fds = *(int *)arg;
	while(1){
		/* Receive */
		byte_recv = recv(fds, buffer, sizeof(buffer), 0);

		if (byte_recv < 0){
			printf("Receive Failed\n");
			break;
		}
		else if(byte_recv == 0){
			printf("Disconnect!\n");
			pthread_exit(NULL);
			close(sock);
			break;
		}
		char s_count[3]={0},buffer1[50]={0};
		s_count[0]='0'+(fds-3)/10;
		s_count[1]='0'+(fds-3)%10;
		s_count[2]='\0';
		strcat(buffer1,"client");
		strcat(buffer1,s_count);
		strcat(buffer1,":");
		strcat(buffer1,buffer);
		printf("%s\n",buffer1);
		
		for(int i=0; i< MaxClientSize; i++)
		{
			if(fds != client_id[i])	
				byte_sent = send(client_id[i], buffer1, sizeof(buffer), 0);
		}
		/*
		for(int i = 0; i < count+4; i++) {
			if(clients[i] > 0){
				byte_sent = send(i, buffer, sizeof(buffer), 0);
				if (byte_sent < 0)
				printf("Error sending packet\n");
			}
		}*/
	}
	return 0;
}

int main(int argc, char *argv[]){
	
	bzero(&server_addr, server_addr_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PortNumber);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)	printf("Error creating socket\n");
    //else	printf("Socket creating successfully\n");

    if (bind(sock, (struct sockaddr*)&server_addr, server_addr_length) == -1){
	    printf("Bind error!\n");
		close(sock);
		exit(-1);
    }
    //else printf("Bind successfully.\n");

	if(listen(sock, 20) == -1){
		printf("Listen failed\n");
		close(sock);
	}
	//else printf("Listen successfully.\n");

	while(1){
		if((recfd = accept(sock, (struct sockaddr *)&client_addr, &client_addr_length)) == -1){
			printf("accept failed\n");
			close(sock);
			break;
		}
		else{
			count++;
			client_id[c_count] = recfd;
			//printf("accept successfully\n");
			printf("client%d connect\n", count);
			c_count++;
		}
		
		clients[recfd] = count;
		
		rc = pthread_create(&thread, NULL, threadFunc, &recfd);
		
		if(rc){
			printf("Error; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}		
	}
    return 0;
};
