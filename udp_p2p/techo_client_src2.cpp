/**********************
project name : p2p_shell
license : MIT License
Author : Ventery
Email :1297054069@qq.com
***********************/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <gcrypt.h>


#define BUF_SIZE 10240
#define MY_PORT 39000
#define MESSAGE_SEND_PERIOD 5
#define KEEP_ALIVE_PERIOD 20

#define CIPHER_ALGO GCRY_CIPHER_AES128 //使用AES128进行加密
const char key[]="5eS3S37S49VBiw2u";
char iv[17]="";
struct connect_info
{
	int sockt;
	char *serv_ip;
	char *serv_port;
	char *message_to_send;
};
int sock,sockt;
int peer_no;
struct connect_info *info;
pthread_t t_id;
pid_t p_id;
size_t key_size;
size_t block_size;
size_t file_size;
size_t block_required;
enum AnalysisState { ANALYSIS_ERROR = 0,  ANALYSIS_SUCCESS};

void error_handling(char *message);
void praseIP_PORT(char *message,int IP ,int PORT );
pid_t get_tid();
void genetate_vec();
void make_message(const char * );
std::string decode(const std::string );
void handle_alarm(int );
void *thread_fun(void * );
void sig_handling(int );
void init_gcrypt();
AnalysisState analysis(const std::string);
int main(int argc, char *argv[])
{
	printf("Main tid is : %d\n",p_id=get_tid());
	signal(SIGINT,sig_handling);
	signal(SIGPIPE,sig_handling);
	int str_len;
	socklen_t adr_sz;

	int option;
	socklen_t optlen;
	optlen=sizeof(option);
	option=true;
	
	struct sockaddr_in serv_adr, from_adr, bind_adr,clnt_adr;
	socklen_t clnt_adr_sz;
	if(argc!=4){
		printf("Usage : %s <IP> <port> <local_port>\n", argv[0]);
		exit(1);
	}
	sock=socket(PF_INET, SOCK_STREAM, 0);  
	sockt=socket(PF_INET, SOCK_STREAM, 0);


	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &option, optlen);
	setsockopt(sockt, SOL_SOCKET, SO_REUSEADDR, &option, optlen);
	setsockopt(sockt, SOL_SOCKET, SO_REUSEPORT, &option, optlen);
	init_gcrypt();

	if(sock==-1)
		error_handling("socket() error");
	memset(&bind_adr, 0, sizeof(bind_adr));

	bind_adr.sin_family=AF_INET;

	bind_adr.sin_addr.s_addr=htonl(INADDR_ANY);

	bind_adr.sin_port=htons(atoi(argv[3]));

	if(bind(sock, (struct sockaddr*)&bind_adr, sizeof(bind_adr))==-1)
		error_handling("bind() error");
	if(bind(sockt, (struct sockaddr*)&bind_adr, sizeof(bind_adr))==-1)
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

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");

	str_len=read(sock, message2, BUF_SIZE);
	close(sock);
	assert(str_len);
	char con_num[10],ip_s[30],port_s[20];
	sscanf(message2,"%s%s%s",con_num,ip_s,port_s);
	peer_no=atoi(con_num);
	printf("you are peer %d\n",peer_no); //peer_no=1 代表服务器，peer_no=2代表客户端
	printf("peer is :%s %s\n",ip_s,port_s);
	memset(message ,0 ,BUF_SIZE);
	memset(message2 ,0 ,BUF_SIZE);

	info=new connect_info;

	info->sockt=sockt;
	info->serv_ip=ip_s;
	info->serv_port=port_s;
	info->message_to_send=message;

	//printf("message : %s\n",message);
	if(pthread_create(&t_id, NULL, thread_fun, (void*)info)!=0)
	{
		puts("pthread_create() error");
		return -1;
	};
#ifdef __linux__
	printf("Linux system!\n");
#elif __APPLE__
	printf("MacOS system!\n");
#endif
	if (peer_no==1)
	{
		pthread_join(t_id,NULL);
		for (;;)
		{
		    char *msg="Please input program you want to run!\n";
			make_message(msg);		
fileNotExist: 
			memset(message ,0 ,BUF_SIZE);
			memset(message2 ,0 ,BUF_SIZE);
			bool is_check=false;
			int pos=0;

			while (!is_check)//检查完整性
			{
				str_len=read(sockt, message2+pos, BUF_SIZE);
				printf("str_len: %d\n",str_len);
				pos+=str_len;
				if (analysis(std::string(message2,pos))) is_check=true;
			}
						printf("1111111\n");

			if (str_len-47<=0) 
			{ 
				//char *msg="Wrong message!!\n";
				//make_message(msg);
				continue;
			}
			std::string tps=decode(std::string(message2,pos));//注意，需要指出长度
			const char* file_name =tps.c_str();
			printf("filename : %s\n",file_name);

            /*判断客户要运行的CGI程序是否存在*/
            if( access(file_name, F_OK) == -1 )
            {
                sprintf(message,"%s is not exist!\n", file_name );
                make_message(message);
                goto fileNotExist;
                continue;
            }
            sprintf(message,"New program is going to run!\n");
            make_message(message);

            /*创建子进程来执行CGI程序*/
            int socket_pair[2]; 
            if(socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair) == -1 ) 
    			error_handling("Error, socketpair create failed");
            int ret=fork();

            if (ret==0)
            {
                /*子进程将标准输出定向到m_sockfd,并执行CGI程序*/
                close( STDIN_FILENO ) ;
                close( STDOUT_FILENO ) ;
                close( STDERR_FILENO ) ;
                dup( socket_pair[1] );
                dup( socket_pair[1] );
                dup( socket_pair[1] );
                execl( file_name, file_name, 0 );
                exit(0);
        	}
        	else 
        	{
        		close(socket_pair[1]);
				for (;;)
				{
					memset(message ,0 ,BUF_SIZE);
					memset(message2 ,0 ,BUF_SIZE);
					bool is_check=false;
					int pos=0,str_len=0;
					while (!is_check)//检查完整性
					{
						str_len=read(sockt, message2+pos, BUF_SIZE);
						pos+=str_len;
						if (analysis(std::string(message2,pos))) is_check=true;
					}
					if (str_len-47<=0) 
					{ 
						//char *msg="Wrong message!!\n";
						//make_message(msg);
						continue;
					}
					std::string tps=decode(std::string(message2,pos));//注意，需要指出长度
					printf("to bash(%d bytes):%s\n",strlen(tps.c_str()),tps.c_str());
					write(socket_pair[0],tps.c_str(),strlen(tps.c_str()));
					write(socket_pair[0],"\n",1);//这里不自带\n要自己加上
					int status;
					if (waitpid(ret,&status,WNOHANG)) 
						{
							make_message("program exited\n");
							break;
						}
					read(socket_pair[0],message,BUF_SIZE);
					make_message(message);
				}
        		close(socket_pair[0]);
        	}
		}
	}
	else pthread_join(t_id,NULL);
	close(sockt);
	return 0;
}


void * thread_fun(void * arg)
{
	printf("in thread tid is %d\n",get_tid());
	info=(struct connect_info*) arg;
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(info->serv_ip);
	serv_addr.sin_port=htons(atoi(info->serv_port));
	sleep(1);
	if (connect(info->sockt, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
			error_handling("connect() error!");
	else 
	{
		printf("Connect success!\n");
		if (peer_no==2) 
		{
			char message[BUF_SIZE];
			char message2[BUF_SIZE];
			//printf("%s\n",message);
			//连接成功后，只从后一个接入的client写，只从前一个接入的client读（此时为server）
			//tcp保活（防止nat自行消除记录）使用自定义应用层协议，因为tcp自带的keepalive套接字选项不够灵活，需要设置内核参数，并且tcp连接完好不一定应用层也正常
			//加密使用AES128算法
			signal(SIGALRM,handle_alarm);// 10秒发射一个保活报文
			unsigned seed;
			srand(seed);
			alarm(KEEP_ALIVE_PERIOD);
			for (;;)
			{
				memset(message,0,BUF_SIZE);
				memset(message2,0,BUF_SIZE);
				bool is_check=false;
				int strlen=0,pos=0;
				while (!is_check)//检查完整性
				{
					strlen=read(info->sockt, message+pos, BUF_SIZE);
					pos+=strlen;
					if (analysis(std::string(message,pos))) is_check=true;
				}
				std::string tps=decode(std::string(message,pos));//注意，需要指出长度
				printf("received (%d bytes) \n%s\n",tps.size(),tps.c_str());
				gets(message);
				make_message(message);
			}
		}
		else ;//close(info->sockt);
	}
}
void make_message(const char * buf)
{
	printf("make_message (%d bytes): %s\n",strlen(buf),buf);

	std::string message="HPTP/1.0\r\n";
	iv[16]='\0';
	genetate_vec();
	//printf("iv : %s\n",iv);
	message=message+"iv: "+iv+"\r\n";
	//encode begin
	gcry_cipher_hd_t cipher_hd;
	gcry_error_t cipher_err;
	file_size = strlen(buf);
	block_required=file_size/block_size;
	if (file_size % block_size != 0){
	    block_required++;
	}

	char cipher_buffer[block_required*block_size];
	memset(cipher_buffer,0,block_required*block_size);
	if (file_size)
	{
		cipher_err=gcry_cipher_open(&cipher_hd, CIPHER_ALGO, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_CBC_CTS);
		cipher_err=gcry_cipher_setkey(cipher_hd,key,key_size);
		cipher_err=gcry_cipher_setiv(cipher_hd, iv, block_size);

		cipher_err=gcry_cipher_setiv(cipher_hd, iv, block_size);

		memcpy(cipher_buffer,buf,file_size);
		//printf("%s\n",cipher_buffer);
		cipher_err=gcry_cipher_encrypt(cipher_hd,cipher_buffer,block_required*block_size,NULL,0);
		//printf("%s\n",cipher_buffer);
		gcry_cipher_close(cipher_hd);

	}
	//encode over
	message=message+"length: "+std::to_string(block_required*block_size)+"\r\n\r\n";
	if (file_size)
		//message=message+cipher_buffer+"\r\n";//错误，应使用下一行。因为cipher_buffer加密后字符串中可能有\0，所以需要明确指明长度
		message+=std::string(cipher_buffer,block_required*block_size)+"\r\n";
	else message+="\r\n";
	write(info->sockt,message.c_str(),message.length());
}
std::string decode(const std::string msg)
{
	int pos=msg.find("iv: ");
	int pos_h=msg.find("length: ",pos);
	int pos_t=msg.find("\r\n",pos_h);
	memcpy(iv,msg.c_str()+pos+4,block_size); //取出iv拷贝一下
	int length=atoi(msg.substr(pos_h+8,pos_t-pos_h-8).c_str());
	char decode_buf[length];
	if (length)
	{	
		gcry_cipher_hd_t cipher_hd;
		gcry_error_t cipher_err;
		cipher_err=gcry_cipher_open(&cipher_hd, CIPHER_ALGO, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_CBC_CTS);
		cipher_err=gcry_cipher_setkey(cipher_hd,key,key_size);
		cipher_err=gcry_cipher_setiv(cipher_hd, iv, block_size);
		memcpy(decode_buf,msg.c_str()+pos_t+4,length);
		cipher_err=gcry_cipher_decrypt(cipher_hd,decode_buf,length,NULL,0);
		gcry_cipher_close(cipher_hd);
	}
	else 
	{
		printf("keep alive msg!\n");
	}
	return  length?std::string(decode_buf,length):std::string("");
}
AnalysisState analysis(const std::string buf)
{
	if (buf.find("HPTP/1.0\r\n")==std::string::npos) return ANALYSIS_ERROR;
	int pos=buf.find("iv: ");
	if (pos==std::string::npos) return ANALYSIS_ERROR;
	int pos_h=buf.find("length: ",pos);
	if (pos_h==std::string::npos) return ANALYSIS_ERROR;
	int pos_t=buf.find("\r\n",pos_h);

	if (pos_t==std::string::npos) return ANALYSIS_ERROR;
	int length=atoi(buf.substr(pos_h+8,pos_t-pos_h-8).c_str());
	if (buf.size()!=pos_t+4+length+2) return ANALYSIS_ERROR;
	return ANALYSIS_SUCCESS;
}
void error_handling(char *message)
{
	perror(message);
	printf("error_handling tid: %d \n",get_tid());
	close(sock);
	close(sockt);
	if (p_id==get_tid())
	{
		sleep(2);
		exit(1);
	}
}

pid_t get_tid()
{
#ifdef __linux__
	return static_cast<pid_t>(syscall(SYS_gettid));
#elif __APPLE__
	uint64_t id;
	pthread_threadid_np(0, &id);
	return static_cast<pid_t>(syscall(SYS_thread_selfid));
#endif
}
void genetate_vec()//iv向量由大小写字母和数字组成
{
	for (int i=0;i<block_size;i++)
	{
		int k=rand()%62;
		if (k<26)
			iv[i]=char(k+65);
		else if (k<52) iv[i]=char(k+71);
		else iv[i]=char(k-4);
	}
}

void handle_alarm(int sig)
{
	make_message("");
	alarm(KEEP_ALIVE_PERIOD);
}

void sig_handling(int sig)
{
	printf("sig_handling tid: %d \n",get_tid());
	if (sig==SIGINT)
	{
		printf("SIGINT recieved!\n");
		close(sock);
		close(sockt);
	}
	else if (sig==SIGPIPE)
	{
		printf("SIGPIPE recieved!\n");
		close(sock);
		close(sockt);
	}
	else 
		printf("signal: %d recieved!\n",sig);
	sleep(2);
	exit(1);
}
void init_gcrypt()
{
	key_size = gcry_cipher_get_algo_keylen(CIPHER_ALGO);
	block_size = gcry_cipher_get_algo_blklen(CIPHER_ALGO);
}