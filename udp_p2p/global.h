/**********************
project name : p2p_shell
license : MIT License
Author : Ventery
Email :1297054069@qq.com
***********************/
#pragma once
#include <gcrypt.h>
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

#define BUF_SIZE 10240
#define MY_PORT 39000
#define MESSAGE_SEND_PERIOD 5
#define KEEP_ALIVE_PERIOD 20
#define CIPHER_ALGO GCRY_CIPHER_AES128 //使用AES128进行加密

const char key[]="5eS3S37S49VBiw2u";//AES密钥
const size_t key_size = gcry_cipher_get_algo_keylen( CIPHER_ALGO ) ;
const size_t block_size = gcry_cipher_get_algo_blklen( CIPHER_ALGO );

enum AnalysisState { ANALYSIS_ERROR = 0,  ANALYSIS_SUCCESS };


struct connect_info
{
	int sockt;
	char *serv_ip;
	char *serv_port;
};
char iv[17]="";
int sock,sockt;
int peer_no;
struct connect_info *info;
pthread_t t_id;
pid_t p_id;
