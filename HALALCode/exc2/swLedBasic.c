#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 1024

int main()
{
    printf("nu starter jeg");

    int fd, num_read;
	char buf[128];
    char filePath[] = "/sys/class/gpio/gpio12/value";
	char expectedBytes = 1;
    



	
	// ---- Open file
    fd = open(filePath, O_RDWR);
    if (fd == -1) // Error
    {
        printf("Failed to open with err: %s", strerror(errno));
        return -1;
    }

	// ---- Read file
	num_read = read(fd, buf, expectedBytes);
	if (num_read == -1) // Error
	{
		printf("Failed to read with err: %s", strerror(errno));
		return num_read; // Return err
	}
	else if(num_read < expectedBytes) // Error
	{
		printf("Read less than requested, only %i bytes", num_read);
	}

	buf[num_read] = 0; // Add zero-termination for string
	printf("Read: %s", buf);
}
