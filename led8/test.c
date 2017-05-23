#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME "/dev/led8"
#define ON 0
#define OFF 1
#define LEDDEV_IOC_MAGIC 'x'
#define LEDDEV_IOCUSERLED1 _IOW(LEDDEV_IOC_MAGIC,1,char)
#define LEDDEV_IOCUSERLED2 _IOW(LEDDEV_IOC_MAGIC,2,char)
#define LEDDEV_IOCRED _IOW(LEDDEV_IOC_MAGIC,3,char)
#define LEDDEV_IOCGREEN _IOW(LEDDEV_IOC_MAGIC,4,char)
#define LEDDEV_IOCBLUE _IOW(LEDDEV_IOC_MAGIC,5,char)
#define LEDDEV_IOC_MAXNR 5

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
		ioctl(fd,LEDDEV_IOCUSERLED1,&arg);
		sleep(1);
		arg = OFF;
		ioctl(fd,LEDDEV_IOCUSERLED1,&arg);

		arg = ON;
		ioctl(fd,LEDDEV_IOCUSERLED2,&arg);
		sleep(1);
		arg = OFF;
		ioctl(fd,LEDDEV_IOCUSERLED2,&arg);

		arg = ON;
		ioctl(fd,LEDDEV_IOCRED,&arg);
		sleep(1);
		arg = OFF;
		ioctl(fd,LEDDEV_IOCRED,&arg);

		arg = ON;
		ioctl(fd,LEDDEV_IOCGREEN,&arg);
		sleep(1);
		arg = OFF;
		ioctl(fd,LEDDEV_IOCGREEN,&arg);

		arg = ON;
		ioctl(fd,LEDDEV_IOCBLUE,&arg);
		sleep(1);
		arg = OFF;
		ioctl(fd,LEDDEV_IOCBLUE,&arg);
	}
}
