#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME "/dev/led7"
#define ON 0
#define OFF 1
#define USER_LED_1 0
#define USER_LED_2 1
#define RED 5
#define GREEN 3
#define BLUE 4

int fd;

int main()
{
	char arg=0;
	fd = open(FILENAME,O_RDWR);
	if(fd<0){
		printf("open faild\n");
		return -1;
	}
	while(1){

		arg = ON;
		ioctl(fd,USER_LED_1,&arg);
		sleep(1);
		arg = OFF;
		ioctl(fd,USER_LED_1,&arg);
		arg = ON;
		ioctl(fd,USER_LED_2,&arg);
		sleep(1);
		arg = OFF;
		ioctl(fd,USER_LED_2,&arg);

		arg = ON;
		ioctl(fd,RED,&arg);
		sleep(1);
		arg = OFF;
		ioctl(fd,RED,&arg);/**/
	}
}
