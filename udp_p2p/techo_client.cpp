/**********************
project name : p2p_shell
license : MIT License
Author : Ventery
Email :1297054069@qq.com
***********************/


#include "utility.h"


void * thread_fun(void * arg);
int main(int argc, char *argv[])
{
	printf("Main tid is : %d\n", p_id = get_tid());
	signal(SIGINT, sig_handling);
	signal(SIGPIPE, sig_handling);
	int str_len;
	socklen_t adr_sz;

	int option;
	socklen_t optlen;
	optlen = sizeof(option);
	option = true;

	struct sockaddr_in serv_adr, from_adr, bind_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	if (argc != 4) {
		printf("Usage : %s <IP> <port> <local_port>\n", argv[0]);
		exit(1);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	sockt = socket(PF_INET, SOCK_STREAM, 0);
	if (sockt == -1)
		error_handling("socket() error");

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &option, optlen);
	setsockopt(sockt, SOL_SOCKET, SO_REUSEADDR, &option, optlen);
	setsockopt(sockt, SOL_SOCKET, SO_REUSEPORT, &option, optlen);

	memset(&bind_adr, 0, sizeof(bind_adr));

	bind_adr.sin_family = AF_INET;

	bind_adr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind_adr.sin_port = htons(atoi(argv[3]));

	if (bind(sock, (struct sockaddr*)&bind_adr, sizeof(bind_adr)) == -1)
		error_handling("bind() error");
	if (bind(sockt, (struct sockaddr*)&bind_adr, sizeof(bind_adr)) == -1)
		error_handling("bind() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	int i;

	char *message = ( char *)malloc(BUF_SIZE);
	char *message2 = ( char *)malloc(BUF_SIZE);
	memset(message , 0 , BUF_SIZE);
	memset(message2 , 0 , BUF_SIZE);

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	str_len = read(sock, message2, BUF_SIZE);
	close(sock);
	assert(str_len);
	char con_num[10], ip_s[30], port_s[20];
	sscanf(message2, "%s%s%s", con_num, ip_s, port_s);
	peer_no = atoi(con_num);
	printf("you are peer %d\n", peer_no); //peer_no=1 代表服务器，peer_no=2代表客户端
	printf("peer is :%s %s\n", ip_s, port_s);
	memset(message , 0 , BUF_SIZE);
	memset(message2 , 0 , BUF_SIZE);

	struct connect_info info;
	info.sockt = sockt;
	info.serv_ip = ip_s;
	info.serv_port = port_s;
	//printf("message : %s\n",message);
	if (peer_no == 2 && pthread_create(&t_id, NULL, thread_fun, (void*)&info) != 0)
	{
		puts("pthread_create() error");
		return -1;
	};
#ifdef __linux__
	printf("Linux system!\n");
#elif __APPLE__
	printf("MacOS system!\n");
#endif
	if (peer_no == 1)
	{
		pthread_join(t_id, NULL);
		for (;;)
		{
			char *msg = "Please input program you want to run!\n";
			make_message(msg);
fileNotExist:
			printf("11111111");
			memset(message , 0 , BUF_SIZE);
			memset(message2 , 0 , BUF_SIZE);
			bool is_check = false;
			int pos = 0;
			while (!is_check)//检查完整性
			{
				str_len = read(sockt, message2 + pos, BUF_SIZE);
				pos += str_len;
				if (analysis(std::string(message2, pos))) is_check = true;
			}
			if (str_len - 47 <= 0)
			{
				//char *msg="Wrong message!!\n";
				//make_message(msg);
				continue;
			}
			std::string tps = decode(std::string(message2, pos)); //注意，需要指出长度
			const char* file_name = tps.c_str();
			printf("filename : %s\n", file_name);

			/*判断客户要运行的CGI程序是否存在*/
			if ( access(file_name, F_OK) == -1 )
			{
				sprintf(message, "%s is not exist!\n", file_name );
				make_message(message);
				goto fileNotExist;
				continue;
			}
			sprintf(message, "New program is going to run!\n");
			make_message(message);

			/*创建子进程来执行CGI程序*/
			int socket_pair[2];
			if (socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair) == -1 )
				error_handling("Error, socketpair create failed");
			int ret = fork();

			if (ret == 0)
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
					memset(message , 0 , BUF_SIZE);
					memset(message2 , 0 , BUF_SIZE);
					bool is_check = false;
					int pos = 0, str_len = 0;
					while (!is_check)//检查完整性
					{
						str_len = read(sockt, message2 + pos, BUF_SIZE);
						pos += str_len;
						if (analysis(std::string(message2, pos))) is_check = true;
					}
					if (str_len - 47 <= 0)
					{
						//char *msg="Wrong message!!\n";
						//make_message(msg);
						continue;
					}
					std::string tps = decode(std::string(message2, pos)); //注意，需要指出长度
					printf("to bash(%d bytes):%s\n", strlen(tps.c_str()), tps.c_str());
					write(socket_pair[0], tps.c_str(), strlen(tps.c_str()));
					write(socket_pair[0], "\n", 1); //这里不自带\n要自己加上
					int status;
					if (waitpid(ret, &status, WNOHANG))
					{
						make_message("program exited\n");
						break;
					}
					read(socket_pair[0], message, BUF_SIZE);
					make_message(message);
				}
				close(socket_pair[0]);
			}
		}
	}
	else pthread_join(t_id, NULL);
	close(sockt);
	return 0;
}


void * thread_fun(void * arg)
{
	printf("in thread tid is %d\n", get_tid());
	info = (struct connect_info*) arg;
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(info->serv_ip);
	serv_addr.sin_port = htons(atoi(info->serv_port));
	sleep(1);
	if (connect(info->sockt, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	else
	{
		printf("Connect success!\n");
		if (peer_no == 2)
		{
			char message[BUF_SIZE];
			char message2[BUF_SIZE];
			//printf("%s\n",message);
			//连接成功后，只从后一个接入的client写，只从前一个接入的client读（此时为server）
			//tcp保活（防止nat自行消除记录）使用自定义应用层协议，因为tcp自带的keepalive套接字选项不够灵活，需要设置内核参数，并且tcp连接完好不一定应用层也正常
			//加密使用AES128算法
			signal(SIGALRM, handle_alarm); // 10秒发射一个保活报文
			unsigned seed;
			srand(seed);
			alarm(KEEP_ALIVE_PERIOD);
			for (;;)
			{
				memset(message, 0, BUF_SIZE);
				memset(message2, 0, BUF_SIZE);
				bool is_check = false;
				int strlen = 0, pos = 0;
				while (!is_check)//检查完整性
				{
					strlen = read(info->sockt, message + pos, BUF_SIZE);
					pos += strlen;
					if (analysis(std::string(message, pos))) is_check = true;
				}
				std::string tps = decode(std::string(message, pos)); //注意，需要指出长度
				printf("received (%d bytes) \n%s\n", tps.size(), tps.c_str());
				gets(message);
				make_message(message);
			}
		}
		else ;//close(info->sockt);
	}
}