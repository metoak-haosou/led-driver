#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME "/dev/led6"
#define ON 1
#define OFF 0
#define USER_LED_1 0
#define USER_LED_2 1
#define RGB 2

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
		ioctl(fd,RGB,&arg);
		arg++;
	}
}
