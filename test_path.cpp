#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
 
char * get_exe_path( char * buf, int count)
{
    int i;
    int rslt = readlink("/proc/self/exe", buf, count - 1);
    printf("buf = %s\n",buf);   //如果可执行文件是a，那么就是/home/yi/C++/a
    if (rslt < 0 || (rslt >= count - 1))
    {
        return NULL;
    }
    buf = dirname(buf);   //获取buf的目录部分，为/home/yi/C++
    printf("buf = %s\n",buf);
 
    for (i = rslt; i >= 0; i--)  //得到可执行文件所在目录的上一层目录
    {
        if (buf[i] == '/')
        {
            buf[i + 1] = '\0';
            break;
        }
    }
    return buf;   //buf为/home/yi
}
 
int main(int argc, char ** argv)
{
    char path[1024];
    printf("%s\n", get_exe_path(path, 1024));
    return 0;
}