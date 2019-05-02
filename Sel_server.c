#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#define PortNumber 6000

int main(int argc, char *argv[])
{
        struct sockaddr_in server_addr,client_addr;
        int sock, byte_recv, recfd, pos[10]={0}, j, fdmax;
        socklen_t server_addr_length = sizeof(server_addr),client_addr_length = sizeof(client_addr);
        char buffer[50];
        char name[50];
        fd_set master;
        fd_set read_fds;
        FD_ZERO(&master);
        FD_ZERO(&read_fds);
        sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock < 0){   
			printf("Error creating socket\n");
			close(sock);
        }
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PortNumber);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        if(bind(sock,(struct sockaddr *)&server_addr,server_addr_length) == -1){
            printf("error binding\n");
            close(sock);
        }
        if(listen(sock,20) == -1){
            printf("listen failed!\n");
            close(sock);
        }
        FD_SET(sock,&master);
        fdmax=sock;
        while(1){
            read_fds = master ;
            if(select(fdmax+1,&read_fds,NULL,NULL,NULL)==-1){
                printf("Select error \n");
                exit(1);
            }
            for(int i=0;i<=fdmax;i++){
                if(FD_ISSET(i,&read_fds)){
                    if(i==sock){
                        recfd = accept(sock,(struct sockaddr *)&client_addr,&client_addr_length);
                        if(recfd==-1){
                            printf("accept failed!\n");
                            close(sock);
                        }
                        else{						
							FD_SET(recfd,&master);
							pos[recfd-4]=recfd-3;
							if(recfd > fdmax){
							fdmax=recfd;
							}
							printf("client%d connect\n",recfd-3);
                        }
					}//end if with line 79
                    else{  
                        char clientnum[5] = {pos[i-4]+'0'}; 
                        //byte_recv=recv(i,buffer,sizeof(buffer),0);
                        //printf("%s\n",buffer);
                        bzero(&buffer,sizeof(buffer));
                        if((byte_recv=recv(i,buffer,sizeof(buffer),0)) <= 0){
                            printf("receive error !");
                            close(i); // bye!
							FD_CLR(i, &master); // 從 master set 中移除
                        }                       
                        else{
							printf("%s\n",buffer);
                            bzero(&name,sizeof(name));
                            strcat(name, "client");
                            strcat(name,clientnum);
                            strcat(name, ":");
                            strcat(name, buffer);
                            //printf("%s\n",name);
                            for(j = 0; j <= fdmax; j++){
                                if(FD_ISSET(j,&master)){
                                    if(j!=sock && j!=i){
                                        send(j,name,sizeof(name),0);
                                    }
                                }
                            }//end for with line 85
                            bzero(&name,sizeof(name));
                        }//end else with line 106
                    }//end else with line 67                
                }//end if with line 51
            }//end for with line 50
		}//end while with line 44
        close(sock);
        return 0;
}
