#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/uio.h>

#define SOCKET_BUFFER_SIZE      (32768U)


void *thread_function(void *arg)
{
    int len = 0;
    int fd  = *((int*)(arg));
    char buf[500];
    int cnt = 0;

    /*主线程*/
    while(1)
    {        
        /*向main thread线程发送数据*/
        len = sprintf(buf, "Hi, main process, cnt = %d", cnt++);
        write(fd, buf, len);
    
        /*读数据*/
        len = read(fd, buf, 500);
        buf[len]='\0';
        printf("%s\n\n",buf);
        
        sleep(2);    
    } 
    
    return NULL;
}


int main()
{
    int ret;
    int sockets[2];
    int bufferSize = SOCKET_BUFFER_SIZE;
    pthread_t thread;
    
    ret = socketpair(AF_LOCAL, SOCK_SEQPACKET, 0, sockets);
    if(ret == -1)
    {
        printf("socketpair create error!\n");
        return -1;
    }
    
    
    /*设置socket描述符的选项*/
    setsockopt(sockets[0], SOL_SOCKET, SO_SNDBUF, &bufferSize, sizeof(bufferSize));
    setsockopt(sockets[0], SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));
    setsockopt(sockets[1], SOL_SOCKET, SO_SNDBUF, &bufferSize, sizeof(bufferSize));
    setsockopt(sockets[1], SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));

    
    /*创建线程1*/
    pthread_create(&thread, NULL, thread_function, (void*)(&sockets[1]));


    int len = 0;
    int fd  = sockets[0];
    char buf[500];
    int cnt = 0;

    /*主线程*/
    while(1)
    {
        /*读数据*/
        len = read(fd, buf, 500);
        buf[len]='\0';
        printf("%s\n",buf);
        
        /*项thread线程发送数据*/
        len = sprintf(buf, "Hi, thread process, cnt = %d", cnt++);
        write(fd, buf, len);
    }

    return 0;
}
