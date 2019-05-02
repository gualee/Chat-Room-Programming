#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include  <arpa/inet.h>
#include <sys/select.h>
#define MAXLINE 1024
#define SERV_PORT 6000
#define Server_Address "127.0.0.1"

void sendrecv( int connfd ){
    FILE * fp = stdin;
    int   lens;
    char send[MAXLINE];
    char recv[MAXLINE];
    fd_set rset;
    FD_ZERO( &rset );
    int maxfd = ( fileno( fp ) > connfd ? fileno( fp ) : connfd  + 1 );   
    int n;
   
    while(1){
        FD_SET( fileno( fp ), &rset );
        FD_SET( connfd, &rset );
        if( select( maxfd, &rset, NULL, NULL, NULL ) == -1 ){
            printf("Error.\n");
            exit(EXIT_FAILURE  );
        }     
        if( FD_ISSET( connfd, &rset ) ){
            //printf( "client get from server ...\n" );
            memset( recv, 0, sizeof( recv ) );
            n = read( connfd, recv, MAXLINE );
            if( n == 0 ){
                printf("Recv ok.\n");
                break;
            }
            else if( n == -1 ){
                printf("Recv error.\n");
                break;
            }
            else{
                lens = strlen( recv );
                recv[lens] = '\0';
                write( STDOUT_FILENO, recv, MAXLINE );
                printf("\n");
            }
        }//end if with line 54
       
        if( FD_ISSET( fileno( fp ), &rset)){
            memset( send, 0, sizeof( send ) );
            if( fgets( send, MAXLINE, fp ) == NULL ){
                printf("End.\n");
                exit( EXIT_FAILURE );
            }
            else{
                lens = strlen( send );
                send[lens-1] = '\0';
                if( strcmp( send, "q" ) == 0 ){
                    printf( "End.\n" );
                    return;
                }
                //printf("Client send : %s\n", send);
                write( connfd, send, strlen( send ) );
            }
        }//end if with line 75
    }//end while with line 47
}//end function with line 37

int main( int argc, char ** argv ){
    
    struct sockaddr_in address;
    int address_length = sizeof(address);
    int sock;
    
    bzero(&address, address_length);
    address.sin_family = AF_INET;
    address.sin_port = htons(SERV_PORT);
    address.sin_addr.s_addr = inet_addr(Server_Address);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        printf("Error creating socket\n");
    
    if(connect(sock, (struct sockaddr *)&address, address_length) == -1){
		printf("connect failed!\n");
		close(sock);
	}
	
    sendrecv(sock);
    close(sock);
    return 0;
}//end main with line 95
