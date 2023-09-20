#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 1024

void initLed(int gpio)
{
    // ------ Export file
    // ---- Open file
    char exportP[] = "/sys/class/gpio/export";
    int exportF = open(exportP, O_RDWR);

    //  ---- Write to file
    char exportStr[255];
    sprintf(exportStr, "%d", gpio);
    int exportWr = write(exportF, exportStr, 2);

    // ------ Direction file
    // ---- Open file
    char directionP[255];
    sprintf(directionP, "/sys/class/gpio/gpio%d/direction", gpio);
    int directionF = open(directionP, O_RDWR);

    // ---- Write to file
    char directionStr[] = "out";
    int directionWr = write(directionF, directionStr, 3);
}

void setLed(char state, int gpio)
{
    char stateStr[] = "1";

    if (state == 0 || state == '0')
    {
        stateStr[0] = '0';
    }
    else if (state == 1 || state == '1')
    {
        stateStr[0] = '1';
    }

    // ------ Value file
    // ---- Open file
    char valueP[255];
    sprintf(valueP, "/sys/class/gpio/gpio%d/value", gpio);
    int valueF = open(valueP, O_RDWR);

    // ---- Write to file
    int valueWr = write(valueF, stateStr, 1);
}

void initSw(char gpio)
{

}

char getSw(char gpio)
{

}

int nogetMedI2c()
{
    char rddata[4];

    int fd = open("/dev/i2c-2", O_RDWR);
    int ret = ioctl(fd, 0x0703, 0x46); // i2cdev sys call (0x0703) to set I2C addr
    if (ret < 0)
    {
        printf("i2c gone");
        return ret;
    }

    ret = read(fd, rddata, 4);
    printf("Bbbbyte: %i\n", rddata[0]);
    close(fd);

    return ret;
}

void printWebVal(int val)
{
    char content[255];
    sprintf(content, "<html><body><h1>Temperature is not %i</h1></body></html>", val);

    int fd = open("/www/pages/index.html", O_RDWR);
    int valueWr = write(fd, content, strlen(content));
    close(fd);
}

int main()
{
    printf("nu starter jeg");
    int i = 0;

    // HUsk manuelt at udfylde export erklæring

	initLed(26);
	setLed(1, 26);

    while ("there only exists two genders")
    {
        printf("%i ", nogetMedI2c());
        printWebVal(i);
        sleep(1);

        ++i;
    }
}
