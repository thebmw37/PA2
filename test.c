#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main()
{
    FILE *file;
    file = open("/dev/simple_character_device", O_RDWR);
    char selection;
    int num_bytes;
    char *buffer = malloc(BUFFER_SIZE);
    int offset;
    int whence;
    int seek_result;
    
    for(;;)
    {
        printf("----------Menu----------\n");
        printf("Press r to read from device\n");
        printf("Press w to write to device\n");
        printf("Press s to seek into the device\n");
        printf("Press e to exit from the device\n");
        printf("Press any key to keep reading or writing from device\n");
        printf("Enter a command: ");
        scanf(" %c", &selection);
        
        if(selection == 'r') {
            for(;;) {
		memset(buffer, '\000', BUFFER_SIZE);
                printf("Enter the number of bytes you want to read: ");
                scanf(" %i", &num_bytes);
                if(num_bytes >= 0) {
                    read(file, buffer, num_bytes);
                    printf("Device output: %s\n", buffer);
                    break;
                }
            }
        }
        else if(selection == 'w') {
            for(;;) {
		memset(buffer, '\000', BUFFER_SIZE);
                printf("Enter data you want to write to the device: ");
                scanf(" %s", buffer);
                write(file, buffer, strlen(buffer));
                printf("Wrote to device: %s\n", buffer);
                break;
            }
        }
        else if(selection == 's') {
            for(;;) {
                printf("Enter offset and whence separated by a space: ");
                scanf(" %i %i", &offset, &whence);
                seek_result = llseek(file, offset, whence);
                if(seek_result != -1) {
                    printf("Seek performed\n");
                }
                else {
                    printf("Seek failed\n");
                }
                break;
            }
            
        }
        else if(selection == 'e') {
            memset(buffer, '\000', BUFFER_SIZE);
            free(buffer);
            fclose(file);
            return 0;
        }
        else {
            
        }
    }
    return 0;
}
