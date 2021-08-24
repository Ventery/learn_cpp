#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>

#define BUF_SIZE 1024
#define MY_PORT 39000

void error_handling(char *message);
void praseIP_PORT(char *message,int IP ,int PORT );

int main(int argc, char *argv[])
{
	int sock ,sock_tcp;
	int str_len;
	socklen_t adr_sz;
	
	struct sockaddr_in serv_adr, from_adr, bind_adr;
	if(argc!=4){
		printf("Usage : %s <IP> <port> <local_port>\n", argv[0]);
		exit(1);
	}
	sock=socket(PF_INET, SOCK_DGRAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	memset(&bind_adr, 0, sizeof(bind_adr));

	bind_adr.sin_family=AF_INET;

	bind_adr.sin_addr.s_addr=htonl(INADDR_ANY);

	bind_adr.sin_port=htons(atoi(argv[3]));

	if(bind(sock, (struct sockaddr*)&bind_adr, sizeof(bind_adr))==-1)
		error_handling("bind() error");

	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	int i;

	char *message = ( char *)malloc(BUF_SIZE);
	char *message2 = ( char *)malloc(BUF_SIZE);
	memset(message ,0 ,BUF_SIZE);
	memset(message2 ,0 ,BUF_SIZE);
	str_len=sendto(sock, message, strlen(message), 0, 
		(struct sockaddr*)&serv_adr, sizeof(serv_adr));
	str_len=recvfrom(sock, message2, BUF_SIZE, 0, 
		(struct sockaddr*)&from_adr, &adr_sz);
	assert(str_len);
	char ip_s[30],port_s[20];
	printf("peer is :%s\n",message2);
	sscanf(message2,"%s%s",ip_s,port_s);

	serv_adr.sin_addr.s_addr=inet_addr(ip_s);
	serv_adr.sin_port=htons(atoi(port_s));

		


	memset(message ,0 ,BUF_SIZE);
	memset(message2 ,0 ,BUF_SIZE);

	sprintf(message ,"Hello my port is %s\n" , argv[3]);

	for (;;)
	{
		str_len=sendto(sock, message, strlen(message), 0, 
		(struct sockaddr*)&serv_adr, sizeof(serv_adr));

		str_len=recvfrom(sock, message2, BUF_SIZE, 0, 
		(struct sockaddr*)&from_adr, &adr_sz);

		printf("recive messgae :%s\n",message2);
		sleep(1);

	}

	//port[strlen(port)-1]='\0';

	/*
	char *message2 = ( char *)malloc(BUF_SIZE);
	while(++i)
	{
		memset(message ,0 ,BUF_SIZE);
		//memset(message2 ,0 ,BUF_SIZE);
		//fputs("Insert message(q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);       
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
			break;
		str_len=sendto(sock, message, strlen(message), 0, 
					(struct sockaddr*)&serv_adr, sizeof(serv_adr));
		//printf("Sent %d bytes\n",str_len);
		//adr_sz=sizeof(from_adr);
		str_len=recvfrom(sock, message, BUF_SIZE, 0, 
					(struct sockaddr*)&from_adr, &adr_sz);
		//printf("Received %d bytes\n",str_len);
		printf("%s", message);
	}	*/
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
void praseIP_PORT(char *message,int IP ,int PORT )
{

}