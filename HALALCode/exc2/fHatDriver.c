#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "fHatDriver.h"

void initLed(char gpio)
{
    char gpioStr[] = itoa(gpio, gpioStr, 10);

    // ------ Export file
    // ---- Open file
    int exportF = open("/sys/class/gpio/export", O_RDWR);
    if (exportF == -1) // Error
    {
        printf("Failed to open with err: %s", strerror(errno));
        return -1;
    }

    //  ---- Write to file
    int num_wr = write(exportF, gpioStr, 2);
    if (num_wr == -1) // err
    {
        printf("Failed to write with err: %s", strerror(errno));
        return num_wr;
    } 
    else if(num_wr < strlen(gpioStr))
    {
        printf("Did not write the whole string, only %i bytes", num_wr);
    }

    // ------ Direction file
    // ---- Open file
    int directionF = open(strcat(strcat("/sys/class/gpio/gpio", gpioStr), "/direction"), O_RDWR);
    if (directionF == -1) // Error
    {
        printf("Failed to open with err: %s", strerror(errno));
        return -1;
    }

    // ---- Write to file
    char buf[3] = "out";
    int num_wr = write(directionF, buf, 3);
    if (num_wr == -1)
    {
        printf("Failed to write with err: %s", strerror(errno));
        return num_wr;
    } // Return err
    else if(num_wr < strlen(buf))
    {
        printf("Did not write the whole string, only %i bytes", num_wr);
    }
}

void setLed(char state, char gpio)
{
    char stateStr = '0';

    if (state == 0 || state == '0' || state == "off")
    {
        stateStr = '0';
    }
    else if (state == 1 || state == '1' || state == "on")
    {
        stateStr = '1';
    }

    char gpioStr[] = itoa(gpio, gpioStr, 10);

    // ------ Value file
    // ---- Open file
    int valueF = open(strcat(strcat("/sys/class/gpio/gpio", gpioStr), "/value"), O_RDWR);
    if (valueF == -1) // Error
    {
        printf("Failed to open with err: %s", strerror(errno));
        return -1;
    }

    // ---- Write to file
    int num_wr = write(valueF, stateStr, 1);
    if (num_wr == -1)
    {
        printf("Failed to write with err: %s", strerror(errno));
        return num_wr;
    } // Return err
    else if(num_wr < strlen(stateStr))
    {
        printf("Did not write the whole string, only %i bytes", num_wr);
    }
}

void initSw(char gpio)
{

}

char getSw(char gpio)
{

}