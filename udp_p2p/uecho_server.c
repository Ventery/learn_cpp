#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

char ip_arr [2][30];
long port_arr[2];
struct sockaddr_in clnt_adr[2];
int main(int argc, char *argv[])
{
	int serv_sock;
	int str_len;
	socklen_t clnt_adr_sz;
	
	struct sockaddr_in serv_adr;
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
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

	while(1) 
	{
		str_len=recvfrom(serv_sock, message, BUF_SIZE, 0, 
								(struct sockaddr*)&clnt_adr[0], &clnt_adr_sz);
		str_len=recvfrom(serv_sock, message, BUF_SIZE, 0, 
								(struct sockaddr*)&clnt_adr[1], &clnt_adr_sz);
		memset(message2 ,0 ,BUF_SIZE);
		sprintf(message2 ,"%s %5u\n" ,inet_ntoa(clnt_adr[1].sin_addr) ,ntohs(clnt_adr[1].sin_port));//serv_adr里面的东西都是网络序（大端）的
		str_len=sendto(serv_sock, message2, strlen(message2), 0, 
								(struct sockaddr*)&clnt_adr[0], clnt_adr_sz);
		memset(message2 ,0 ,BUF_SIZE);
		sprintf(message2 ,"%s %5u\n" ,inet_ntoa(clnt_adr[0].sin_addr) ,ntohs(clnt_adr[0].sin_port));//serv_adr里面的东西都是网络序（大端）的
		str_len=sendto(serv_sock, message2, strlen(message2), 0, 
								(struct sockaddr*)&clnt_adr[1], clnt_adr_sz);
	}	
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
