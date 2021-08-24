#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#include <sys/stat.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void* arg);
void send_data(FILE* fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(char* message);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_size;
	char buf[BUF_SIZE];
	pthread_t t_id;	
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 20)==-1)
		error_handling("listen() error");

	while(1)
	{
		clnt_adr_size=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
		printf("Connection Request : %s:%d\n", 
			inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
		pthread_create(&t_id, NULL, request_handler, &clnt_sock);
		pthread_detach(t_id);
	}
	close(serv_sock);
	return 0;
}

void* request_handler(void *arg)
{
	int clnt_sock=*((int*)arg);
	char req_line[SMALL_BUF];
	FILE* clnt_read;
	FILE* clnt_write;
	
	char method[10];
	char ct[15];
	char file_name[30];
	int snd_buf;
	int len=sizeof(snd_buf);
	int state=getsockopt(clnt_sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
	printf("Send buf size : %d\n",snd_buf);

	snd_buf=1024*1024*10;
	state=setsockopt(clnt_sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
	printf("Send buf size : %d\n",snd_buf);
	clnt_read=fdopen(clnt_sock, "r");
	clnt_write=fdopen(dup(clnt_sock), "w");
	fgets(req_line, SMALL_BUF, clnt_read);	
	if(strstr(req_line, "HTTP/")==NULL)
	{
		printf("Error1! %s\n",req_line);
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return;
	 }
	printf("%s\n",req_line);
	strcpy(method, strtok(req_line, " /"));
	strcpy(file_name, strtok(NULL, " /"));
	strcpy(ct, content_type(file_name));
	printf("%s %s %s\n",method,file_name,ct);
	if(strcmp(method, "GET")!=0)
	{
		printf("Error2! %s\n",method);
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return;
	 }

	fclose(clnt_read);
	send_data(clnt_write, ct, file_name); 
}

void send_data(FILE* fp, char* ct, char* file_name)
{
	FILE* send_file; 
	send_file=fopen(file_name, "r"); 
	struct stat statbuf;
	stat(file_name,&statbuf);
	int size=statbuf.st_size;
	char protocol[]="HTTP/1.0 200 OK\r\n";
	char server[]="Server:Linux Web Server \r\n";
	char cnt_len[SMALL_BUF];
	sprintf(cnt_len,"Content-length:%d\r\n",size);

	//char cnt_len[]="Content-length:287647\r\n";
	char cnt_type[SMALL_BUF];
	char buf[BUF_SIZE];
	
	sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
	if(send_file==NULL)
	{
		printf("Error3! %s\n",file_name);
		send_error(fp);
		return;
	}
	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
	printf("%s\n%s\n%s\n%s\n",protocol,server,cnt_len,cnt_type);
	int rs;
	while(rs=fread(buf,sizeof(char),BUF_SIZE, send_file)) 
	{
		//printf("%d  %d %d %d %d\n",rs,*(buf+355),*(buf+356),*(buf+357),*(buf+358));
		fwrite(buf,sizeof(char),rs,fp);
		fflush(fp);
	}
	/*while(fgets(buf, BUF_SIZE, send_file)!=NULL) 
	{
		printf("String length : ",strlen(buf));
		fputs(buf, fp);
		fflush(fp);
	}*/

	printf("File over!\n");
	fflush(fp);
	fclose(fp);
}

char* content_type(char* file)
{
	char extension[SMALL_BUF];
	char file_name[SMALL_BUF];
	strcpy(file_name, file);
	strtok(file_name, ".");
	strcpy(extension, strtok(NULL, "."));
	
	if(!strcmp(extension, "html")||!strcmp(extension, "htm")) 
		return "text/html";
	else
		return "image/jpeg";
}

void send_error(FILE* fp)
{	
	printf("Error4!\n");
	char protocol[]="HTTP/1.0 400 Bad Request\r\n";
	char server[]="Server:Linux Web Server \r\n";
	char cnt_len[]="Content-length:2048\r\n";
	char cnt_type[]="Content-type:text/html\r\n\r\n";
	char content[]="<html><head><title>NETWORK</title></head>"
	       "<body><font size=+5><br>Hello World~!"
		   "</font></body></html>";

	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
	fflush(fp);
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
;
}
