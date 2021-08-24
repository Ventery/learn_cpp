#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_CLIENT_NUM 5
#define BUFFER_SIZE 1024
#define SHM_NAME "/test_mmap_s"

int shmfd;
int ret;
char* share_mem = 0;
char message[1024];

int main( int argc, char* argv[] )
{
	if ( argc < 2 )
	{
        printf( "usage: %s client_num\n" , basename( argv[0] ) );
        return 1;
	}

	shmfd = shm_open( SHM_NAME, O_CREAT | O_RDWR , 0666 );
    assert( shmfd != -1 );
    if ( atoi( argv[1] ) == 1 )
    {
    	printf("%d\n",shmfd);
		ret = ftruncate( shmfd, MAX_CLIENT_NUM * BUFFER_SIZE + 1);
		printf("Error number is : %d\n",errno); 
	    assert( ret != -1 );
	    share_mem = (char*)mmap( NULL, MAX_CLIENT_NUM * BUFFER_SIZE + 1 , PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
	    assert( share_mem != MAP_FAILED );
    	for (int i = 0; ;i++)
    	{
    		scanf("%s",message);
    		memcpy(share_mem , message , strlen(message)+1);
    		printf("%d : Send message : %s , length : %d\n",i,message, strlen(message));
    	}
    }
    else 
    {
   		share_mem = (char*)mmap( NULL, MAX_CLIENT_NUM * BUFFER_SIZE + 1 , PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
    	for ( ; ; )
    	{
    		while ( share_mem[0] == '\0' ) { usleep(50); }
    		sscanf( share_mem , "%s" , message );
    		printf( "%s\n" , message );
    		share_mem[0] = '\0';
    	}
    }
    close( shmfd );
	return 0;
}
