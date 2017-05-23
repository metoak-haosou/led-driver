#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME "/dev/led5"
#define ON 0
#define OFF 1
int fd;
int args;
void led_on(int led)
{
	ioctl(fd,ON,&led);
}
void led_off(int led)
{
	args=OFF;
	ioctl(fd,OFF,&led);
}
int main()
{
	fd = open(FILENAME,O_RDWR);
	if(fd<0){
		printf("open faild\n");
		return -1;
	}
	while(1){
		led_off(9);
		led_on(0);
		sleep(1);
		led_off(0);
		led_on(9);
		sleep(1);
	}
}
