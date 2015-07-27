//
//  main.c
//  Last Edit: 7/21/15
//
//  termios : serial i/o with /dev/tty##
//
//  Created by Asher Martin <ashermartin@gmail.com>
//  Help: https://en.wikibooks.org/wiki/Serial_Programming/termios
//
//  Usage:
//    1. "gcc -o term main.c"
//    2. "./term /dev/ttys000"
//    3. "echo "hi" > /dev/ttys000"
//    4. then to quit type "q"
//    5. type "stty sane" to get your terminal back
//  Tested on OSX (XCode 7.0)
//
//  Free and Open Software - No Warrenty
//
//  WARNING: Connect Hardware to your PC or Use Serial termios at Your Own Risk

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h> // needed for struct termios
#include <string.h> // needed for memset

int main(int argc,char** argv)
{
    struct termios tio;
    struct termios stdio;
    int tty_fd;
    fd_set rdset;
    
    unsigned char c='D';
    
    printf("i/o tester! - type 'q' the 'stty sane' to quit \n");
    printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);
    
    
    
    
    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking
    
    
    
    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;
    
    tty_fd=open(argv[1], O_RDWR | O_NONBLOCK);
    cfsetospeed(&tio,B115200);            // 115200 baud
    cfsetispeed(&tio,B115200);            // 115200 baud
    
    tcsetattr(tty_fd,TCSANOW,&tio);
    while (c!='q')
    {
        if (read(tty_fd,&c,1)>0)        write(STDOUT_FILENO,&c,1);              // if new data is available on the serial port, print it out
        if (read(STDIN_FILENO,&c,1)>0)  write(tty_fd,&c,1);                     // if new data is available on the console, send it to the serial port
    }
    
    close(tty_fd);
}
