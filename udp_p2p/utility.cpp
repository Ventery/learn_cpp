/**********************
project name : p2p_shell
license : MIT License
Author : Ventery
Email :1297054069@qq.com
***********************/
#include "utility.h"

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

	size_t file_size;
	size_t block_required;

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