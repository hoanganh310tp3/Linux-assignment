#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_DEVICES 5
#define BUFFER_SIZE 1024

int8_t write_buf[BUFFER_SIZE];
int8_t read_buf[BUFFER_SIZE];

const char *device_names[MAX_DEVICES] = {
    "sensor0", "sensor1", "hello0", "hello1", "goodbye"
};

int main()
{
    int fd;
    int dev_num;
    char device_path[64];
    char option;

    printf("Enter device number to open (0-%d):\n", MAX_DEVICES - 1);
    for (int i = 0; i < MAX_DEVICES; i++) {
        printf("  %d. /dev/%s\n", i, device_names[i]);
    }

    scanf("%d", &dev_num);

    if(dev_num < 0 || dev_num >= MAX_DEVICES) {
        printf("Invalid device number.\n");
        return 1;
    }

    snprintf(device_path, sizeof(device_path), "/dev/%s", device_names[dev_num]);
    fd = open(device_path, O_RDWR);  // Dùng O_RDWR để có thể read + write

    if(fd < 0) {
        perror("Cannot open device file");
        return 1;
    }

    while(1) {
        printf("\n**** Please Enter the Option ****\n");
        printf("        1. Write to device\n");
        printf("        2. Read from device\n");
        printf("        3. Exit\n");
        scanf(" %c", &option);
        getchar(); // Consume newline

        switch(option) {
            case '1':
                printf("Enter something to write into driver: ");
                fgets((char *)write_buf, BUFFER_SIZE, stdin);
                write_buf[strcspn((char *)write_buf, "\n")] = 0; // remove newline

                if(write(fd, write_buf, strlen((char *)write_buf) + 1) < 0)
                    perror("Write failed");
                else
                    printf("Write successful\n");
                break;

            case '2':
                memset(read_buf, 0, BUFFER_SIZE);
                if(read(fd, read_buf, BUFFER_SIZE) < 0)
                    perror("Read failed");
                else
                    printf("Data read: %s\n", read_buf);
                break;

            case '3':
                close(fd);
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid option: %c\n", option);
        }
    }

    close(fd);
    return 0;
}
