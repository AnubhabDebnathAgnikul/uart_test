# Compilers
CCNXP=/home/swami/AgnikulCosmos/gcc-nxp/bin/aarch64-none-linux-gnu-gcc # NXP Compiler
CC=gcc # PC Compiler

all:
	${CC} tx_uart.c -o tx_uart
	${CC} rx_uart.c -o rx_uart

nxp:
	${CCNXP} tx_uart.c -o tx_uart_nxp
	${CCNXP} rx_uart.c -o rx_uart_nxp
	
.PHONY: clean scp

clean:
	rm -rf tx_uart rx_uart 
	rm -rf tx_uart_nxp rx_uart_nxp

scp:
	scp *_nxp root@${IP}:/root/cipher/uart
