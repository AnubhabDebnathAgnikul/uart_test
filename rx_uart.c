/*******************************************************************
 * @author:     Anubhab Debnath <anubhab_debnath@agnikul.in>
 * @author:     Aravind Potluri <aravindpotluri@agnikukl.space>
 * @brief:      Basic UART Transmitter and Receiver code.
 *
 * *********************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) Agnikul Cosmos Private Limited
 * All rights reserved. </center></h2>
 *
 *******************************************************************/

// Headers
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

// Macros
#define BAUDRATE_GIMBAL B230400
#define BAUDRATE_IMU B1152000
#define BAUDRATE_GNSS B921600
#define TRUE 1

// Global variables
char serialPort[20];
uint8_t receiveData;

int main()
{
    // Get the Serial Port
    printf("Enter the Serial Port: ");
    scanf("%s", serialPort);

    // Opening the serial port
    int uartFD = open(serialPort, O_RDWR | O_NOCTTY);
    if (uartFD > 0)
    {
        printf("[+] UART serial port opened successfully.");
    }
    else
    {
        printf("[-] Error in opening UART serial port: %s | %d" , strerror(errno), errno);
        return 1;
    }

    // Configuring the serial port parameters
    struct termios tty;
    tcgetattr(uartFD, &tty);
    cfsetispeed(&tty, BAUDRATE_IMU);
    cfsetospeed(&tty, BAUDRATE_IMU);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;
    tcsetattr(uartFD, TCSANOW, &tty);

    // Receive UART data over serial port
    while (TRUE)
    {
        if (read(uartFD, &receiveData, sizeof(receiveData)) > 0)
        {
            printf("Data Received: %d\n", receiveData);
        }
        else
        {
            printf("[-] Error reading from UART: %s | %d\n", strerror(errno), errno);
            return -1;
        }
    }

    close(uartFD);
    return 0;
}