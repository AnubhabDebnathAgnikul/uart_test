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
#include <stdint.h>
#include <signal.h>
#include <errno.h>

// Macros
#define BAUDRATE_GIMBAL B230400
#define BAUDRATE_IMU B1152000
#define BAUDRATE_GNSS B921600
#define BAUDRATE BAUDRATE_IMU
#define TRUE 1

// Global variables
char serialPort[20];
char sendBuff[1024];
uint8_t loop = 1;

// Functions
void signalHandler()
{
    loop = 0;
}

int main()
{
    // Termination signal
    signal(SIGINT, signalHandler);

    // Get serial port
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
        printf("[-] Error in opening UART serial port: %s | %d", strerror(errno), errno);
        return 1;
    }

    // Configuring the serial port parameters
    struct termios tty;
    tcgetattr(uartFD, &tty);
    cfsetispeed(&tty, BAUDRATE);
    cfsetospeed(&tty, BAUDRATE);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;
    tcsetattr(uartFD, TCSANOW, &tty);

    // Send UART data over serial port
    while (loop)
    {
        printf("Enter the Message: ");
        scanf("%s", sendBuff);
        if (write(uartFD, sendBuff, sizeof(sendBuff)) > 0)
        {
            printf("Data Sent: %s\n", sendBuff);
        }
        else
        {
            printf("Error writing to UART: %s | %d\n", strerror(errno), errno);
            return -1;
        }
        if (sendBuff == "")
        {
            printf("[+] NULL: Termination signal Received.");
            loop = 0;
        } 
        sleep(1);
    }

    close(uartFD);
    return 0;
}
