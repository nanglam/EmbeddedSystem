#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ioctl.h>

#define DEVICE_FILE_NAME = "/dev/VirtualDisk"
#define VIR_DEV_MAJOR = 246
#define WRBUFSIZE _IOW(VIR_DEV_MAJOR, 1, int) 
#define REBUFSIZE _IOR(VIR_DEV_MAJOR, 2, int)


int main(int argc, char **argv) {
    int device;
    if(argc != 3) {
        printf("Usage : ./change_buffer_size [N] [M]")
    }

    char wbuf[128] = "Write buffer data";
    char rbuf[128] = "Read buffer data";
    int n = atoi(argv[1]);

    device = open(DEVICE_FILE_NAME, O_RDWR | O_NDELAY);
    if (device >= 0) {
        printf("ioctl is calld: %d\n", n);
        ioctl(device, WRBUFSIZE, argv[1]);
        ioctl(device, REBUFSIZE, argv[2]);
        

        write(device, wbuf, sizeof(wbuf));
        printf("Write value is %s\n", wbuf);

        read(device, rbuf, sizeof(rbuf));
        printf("Read valuit is %s\n", rbuf);
    } else {
        perror("Device file open fail...\n");
    }

    close(device);
    return 0;
}