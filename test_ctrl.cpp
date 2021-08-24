#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>


#ifdef __linux__
#define TTY_PATH            "/dev/ttys002"
#define STTY_US             "stty raw -echo -F "
#define STTY_DEF            "stty -raw echo -F "
#elif __APPLE__
#define TTY_PATH            " /dev/ttys002"
#define STTY_PREFIX         "stty -f"
#define STTY_US_SUFFIX      " raw -echo"
#define STTY_DEF_SUFFIX     " -raw echo"
#endif


static int get_char();

static int get_char()
{
    fd_set rfds;
    struct timeval tv;
    int ch = 0;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 10; //设置等待超时时间

    //检测键盘是否有输入
    if (select(1, &rfds, NULL, NULL, &tv) > 0)
    {
        ch = getchar(); 
    }

    return ch;
}

int main()
{
    int ch = 0;
#ifdef __linux__
    system(STTY_US TTY_PATH);
#elif __APPLE__
    system(STTY_PREFIX TTY_PATH STTY_US_SUFFIX);
#endif


    while(1)
    {
        ch = get_char();
        if (ch)
        {
            printf("key = %d(%c)\n\r", ch, ch);
            switch (ch)
            {
				case 3  ://ctrl+c
                    {
                        #ifdef __linux__
                            system(STTY_DEF TTY_PATH);
                        #elif __APPLE__
                            system(STTY_PREFIX TTY_PATH STTY_DEF_SUFFIX);
                        #endif
                        return 0;
                    }
                case '0':
                    printf("0\n\r");break;
                case '1':
                    printf("1\n\r");break;
                case '2':
                    printf("2\n\r");break;
                case '3':
                    printf("3\n\r");break;
            }
        }           
    }
}

