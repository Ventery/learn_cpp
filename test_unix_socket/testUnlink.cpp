#include <stdlib.h>  
#include <stdio.h>  
#include <stddef.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <errno.h>  
#include <string.h>  
#include <unistd.h>  
#include <ctype.h>   
 
#define MAXLINE 80  
 
char *socket_path = "server.socket";  
 
int main(void)  
{  
    unlink(socket_path);  //删除文件而已，硬连接减少引用
    return 0;  
}