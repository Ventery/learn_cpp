/**********************
project name : p2p_shell
license : MIT License
Author : Ventery
Email :1297054069@qq.com
***********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define BUF_SIZE 1024
void error_handling(char *message);
void sig_handling(int sig);


char ip_arr [2][30];
long port_arr[2];
struct sockaddr_in clnt_adr[2];
int serv_sock,clnt_sock1,clnt_sock2;
int main(int argc, char *argv[])
{
	signal(SIGINT,sig_handling);
	signal(SIGPIPE,sig_handling);

	int str_len;
	socklen_t clnt_adr_sz;
	int option=1;
	socklen_t optlen=sizeof(option);
	
	struct sockaddr_in serv_adr;
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);

	if(serv_sock==-1)
		error_handling("UDP socket creation error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	//printf("port in net    %d\n",serv_adr.sin_port);//39000(host)->22680(net)
	
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	char *message = ( char *)malloc(BUF_SIZE);
	char *message2 = ( char *)malloc(BUF_SIZE);
	clnt_adr_sz=sizeof(clnt_adr[0]);

	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	while(1) 
	{
		clnt_sock1=accept(serv_sock,  (struct sockaddr*)&clnt_adr[0], &clnt_adr_sz);
		printf("Accpet1 : %s %5u\n",inet_ntoa(clnt_adr[0].sin_addr) ,ntohs(clnt_adr[0].sin_port));
		/*clnt_sock2=accept(serv_sock,  (struct sockaddr*)&clnt_adr[1], &clnt_adr_sz);
		printf("Accpet2 : %s %5u\n",inet_ntoa(clnt_adr[1].sin_addr) ,ntohs(clnt_adr[1].sin_port));
		memset(message2 ,0 ,BUF_SIZE);
		sprintf(message2 ,"%d %s %5u\n" ,1,inet_ntoa(clnt_adr[1].sin_addr) ,ntohs(clnt_adr[1].sin_port));//serv_adr里面的东西都是网络序（大端）的
		str_len=write(clnt_sock1, message2, strlen(message2));
		memset(message2 ,0 ,BUF_SIZE);
		sprintf(message2 ,"%d %s %5u\n" ,2,inet_ntoa(clnt_adr[0].sin_addr) ,ntohs(clnt_adr[0].sin_port));//serv_adr里面的东西都是网络序（大端）的
		str_len=write(clnt_sock2, message2, strlen(message2));*/
		//close(clnt_sock1);
		//close(clnt_sock2);
	}	
	close(serv_sock);
	return 0;
}
void sig_handling(int sig)
{
	if (sig==SIGINT)
	{
		printf("SIGINT recieved!\n");
		close(serv_sock);
		close(clnt_sock1);
		close(clnt_sock2);
		sleep(2);
		printf("exited\n");
		exit(1);
	}
	else if (sig==SIGPIPE)
	{
		printf("SIGPIPE recieved!\n");
		close(clnt_sock1);
		close(clnt_sock2);
	}
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	close(serv_sock);
	close(clnt_sock1);
	close(clnt_sock2);
	sleep(2);
	printf("exited\n");
	exit(1);
}
