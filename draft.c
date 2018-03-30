#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#define FALSE -1
#define TRUE 0

//#include "ser.h"
//115200 8 n 1 


void init_tty(int fd)
{
	struct termios newtio;
	struct termios oldtio;
	bzero(&newtio,sizeof(struct termios));//初始化 全部设置为0

	tcgetattr(fd,&oldtio);//1 数据保存到oldtio

	newtio.c_cflag |= CLOCAL | CREAD;//2接收使能 置1
	cfsetispeed(&newtio,B115200);//3设置输入速度
	cfsetispeed(&newtio,B115200);//3输出速度
	//设置数据位为8位
	printf("c_cflag=%x\n",newtio.c_cflag);
	newtio.c_cflag &= ~CSIZE;//对应比特位清零4
	printf("c_cflag=%x\n",newtio.c_cflag);
	newtio.c_cflag |= CS8;///对应比特位置位4
	printf("c_cflag=%x\n",newtio.c_cflag);
	//设置无校验56
	newtio.c_cflag &= ~PARENB;//|=允许
	newtio.c_iflag &= ~INPCK;
	//设置1位停止位7
	newtio.c_cflag &= !CSTOPB;//默认是2位，取反就是1位
	//设置最少字符与时间8
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;
	//刷清串口缓存9
	tcflush(fd,TCIOFLUSH)；
	//设置新的属性到串口文件10
	tcsetattr(fd,TCSANOW,&newtio);//TCSANOW立即改变
} 


int OpenDev(char *Dev)
{
	int fd = open(Dev,O_RDWR);
	if(-1 == fd)
	{
		perror("Can't Open Serial Port");
		return -1;
	}
	else
		{
			return fd;
		}
}

int main(int argc,char **argv)
{
	int fd;
	int nread;
	char buff[512];
	char *dev = "/dev/ttyS1";////串口二
	fd = OpenDev(dev);
	init_tty(fd);
	while(1)//循环读取数据
	{
		while((nread = read(fd,buff,512))>0)
		{
			printf("\nLen %d\n",nread);
			buff[nread+1] = '\0';
			printf("\n%s",buff);
		}
	}
	close(fd);
	exit(0);

}